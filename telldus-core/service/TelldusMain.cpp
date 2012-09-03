#include "TelldusMain.h"
#include "ConnectionListener.h"
#include "EventHandler.h"
#include "ClientCommunicationHandler.h"
#include "DeviceManager.h"
#include "ControllerManager.h"
#include "ControllerListener.h"
#include "EventUpdateManager.h"
#include "Timer.h"
#include "Log.h"
#include "common.h" //debug

#include <stdio.h>
#include <list>
#include <memory>

class TelldusMain::PrivateData {
public:
	TelldusCore::EventHandler eventHandler;
	TelldusCore::EventRef stopEvent, controllerChangeEvent;
};

TelldusMain::TelldusMain(void)
{
	d = new PrivateData;
	d->stopEvent = d->eventHandler.addEvent();
	d->controllerChangeEvent = d->eventHandler.addEvent();
}

TelldusMain::~TelldusMain(void) {
	delete d;
}

void TelldusMain::deviceInsertedOrRemoved(int vid, int pid, bool inserted) {
	ControllerChangeEventData *data = new ControllerChangeEventData;
	data->vid = vid;
	data->pid = pid;
	data->inserted = inserted;
	d->controllerChangeEvent->signal(data);
}

void TelldusMain::resume() {
	Log::notice("Came back from suspend");
	ControllerChangeEventData *data = new ControllerChangeEventData;
	data->vid = 0x0;
	data->pid = 0x0;
	data->inserted = true;
	d->controllerChangeEvent->signal(data);
}

void TelldusMain::suspend() {
	Log::notice("Preparing for suspend");
	ControllerChangeEventData *data = new ControllerChangeEventData;
	data->vid = 0x0;
	data->pid = 0x0;
	data->inserted = false;
	d->controllerChangeEvent->signal(data);
}

void TelldusMain::start(void) {
	//Logga, starta service
	debuglogservice(0, "Starting service");
	TelldusCore::EventRef clientEvent = d->eventHandler.addEvent();
	TelldusCore::EventRef dataEvent = d->eventHandler.addEvent();
	TelldusCore::EventRef janitor = d->eventHandler.addEvent(); //Used for regular cleanups
	Timer supervisor(janitor); //Tells the janitor to go back to work
	supervisor.setInterval(60); //Once every minute
	supervisor.start();

	EventUpdateManager eventUpdateManager;
	TelldusCore::EventRef deviceUpdateEvent = eventUpdateManager.retrieveUpdateEvent();
	eventUpdateManager.start();
	ControllerManager controllerManager(dataEvent, deviceUpdateEvent);
	DeviceManager deviceManager(&controllerManager, deviceUpdateEvent);

	ConnectionListener clientListener(L"TelldusClient", clientEvent);

	std::list<ClientCommunicationHandler *> clientCommunicationHandlerList;

	TelldusCore::EventRef handlerEvent = d->eventHandler.addEvent();

#ifdef _MACOSX
	//This is only needed on OS X
	ControllerListener controllerListener(d->controllerChangeEvent);
#endif


	while(!d->stopEvent->isSignaled()) {
		if (!d->eventHandler.waitForAny()) {
			continue;
		}
		if (clientEvent->isSignaled()) {
			//New client connection
			TelldusCore::EventDataRef eventDataRef = clientEvent->takeSignal();
			ConnectionListenerEventData *data = reinterpret_cast<ConnectionListenerEventData*>(eventDataRef.get());
			if (data) {
				ClientCommunicationHandler *clientCommunication = new ClientCommunicationHandler(data->socket, handlerEvent, &deviceManager, deviceUpdateEvent, &controllerManager);
				clientCommunication->start();
				clientCommunicationHandlerList.push_back(clientCommunication);
			}
		}

		if (d->controllerChangeEvent->isSignaled()) {
			TelldusCore::EventDataRef eventDataRef = d->controllerChangeEvent->takeSignal();
			ControllerChangeEventData *data = reinterpret_cast<ControllerChangeEventData*>(eventDataRef.get());
			if (data) {
				controllerManager.deviceInsertedOrRemoved(data->vid, data->pid, "", data->inserted);
			}
		}

		if (dataEvent->isSignaled()) {
			TelldusCore::EventDataRef eventData = dataEvent->takeSignal();
			ControllerEventData *data = reinterpret_cast<ControllerEventData*>(eventData.get());
			if (data) {
				deviceManager.handleControllerMessage(*data);
			}
		}

		if (handlerEvent->isSignaled()) {
			handlerEvent->popSignal();
			for ( std::list<ClientCommunicationHandler *>::iterator it = clientCommunicationHandlerList.begin(); it != clientCommunicationHandlerList.end(); ){
				if ((*it)->isDone()){
					delete *it;
					it = clientCommunicationHandlerList.erase(it);

				} else {
					++it;
				}
			}
		}
		if (janitor->isSignaled()) {
			//Clear all of them if there is more than one
			while(janitor->isSignaled()) {
				janitor->popSignal();
			}
			controllerManager.queryControllerStatus();
		}
	}

	supervisor.stop();
}

void TelldusMain::stop(void){
	d->stopEvent->signal();
}
