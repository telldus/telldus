#include "TelldusMain.h"
#include "ConnectionListener.h"
#include "EventHandler.h"
#include "ClientCommunicationHandler.h"
#include "DeviceManager.h"
#include "ControllerManager.h"
#include "EventUpdateManager.h"

#include <stdio.h>
#include <list>
#include <memory>

class DeviceEventData : public EventDataBase {
public:
	int vid, pid;
	bool inserted;
};

class TelldusMain::PrivateData {
public:
	EventHandler eventHandler;
	EventRef stopEvent, deviceChangeEvent;
};

TelldusMain::TelldusMain(void)
{
	d = new PrivateData;
	d->stopEvent = d->eventHandler.addEvent();
	d->deviceChangeEvent = d->eventHandler.addEvent();
}

TelldusMain::~TelldusMain(void) {
	delete d;
}

void TelldusMain::deviceInsertedOrRemoved(int vid, int pid, bool inserted) {
	DeviceEventData *data = new DeviceEventData;
	data->vid = vid;
	data->pid = pid;
	data->inserted = inserted;
	d->deviceChangeEvent->signal(data);
}

void TelldusMain::start(void) {
	EventRef clientEvent = d->eventHandler.addEvent();
	EventRef dataEvent = d->eventHandler.addEvent();

	ControllerManager controllerManager(dataEvent.get());
	EventUpdateManager eventUpdateManager;
	EventRef deviceUpdateEvent = eventUpdateManager.retrieveUpdateEvent();
	eventUpdateManager.start();
	DeviceManager deviceManager(&controllerManager, deviceUpdateEvent);

	ConnectionListener clientListener(L"TelldusClient", clientEvent);

	std::list<ClientCommunicationHandler *> clientCommunicationHandlerList;

	EventRef handlerEvent = d->eventHandler.addEvent();

	while(!d->stopEvent->isSignaled()) {
		if (!d->eventHandler.waitForAny()) {
			continue;
		}
		if (clientEvent->isSignaled()) {
			//New client connection
			EventDataRef eventDataRef = clientEvent->takeSignal();
			ConnectionListenerEventData *data = reinterpret_cast<ConnectionListenerEventData*>(eventDataRef.get());
			if (data) {
				ClientCommunicationHandler *clientCommunication = new ClientCommunicationHandler(data->socket, handlerEvent, &deviceManager, deviceUpdateEvent);
				clientCommunication->start();
				clientCommunicationHandlerList.push_back(clientCommunication);
			}
		}

		if (d->deviceChangeEvent->isSignaled()) {
			EventDataRef eventDataRef = d->deviceChangeEvent->takeSignal();
			DeviceEventData *data = reinterpret_cast<DeviceEventData*>(eventDataRef.get());
			if (data) {
				controllerManager.deviceInsertedOrRemoved(data->vid, data->pid, data->inserted);
			}
		}

		if (dataEvent->isSignaled()) {
			EventDataRef eventData = dataEvent->takeSignal();
			ControllerEventData *data = reinterpret_cast<ControllerEventData*>(eventData.get());
			if (data) {
				deviceManager.handleControllerMessage(*data);
			}
		}

		if(handlerEvent->isSignaled()){
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
	}
}

void TelldusMain::stop(void){
	d->stopEvent->signal();
}
