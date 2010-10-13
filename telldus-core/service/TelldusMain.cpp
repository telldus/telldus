#include "TelldusMain.h"
#include "ConnectionListener.h"
#include "EventHandler.h"
#include "ClientCommunicationHandler.h"
#include "DeviceManager.h"
#include "ControllerManager.h"

#include <stdio.h>
#include <list>

class TelldusMain::PrivateData {
public:
	EventHandler eventHandler;
	Event *stopEvent;
};

TelldusMain::TelldusMain(void)
{
	d = new PrivateData;
	d->stopEvent = d->eventHandler.addEvent();
}

TelldusMain::~TelldusMain(void) {
	delete d->stopEvent;
	delete d;
}

void TelldusMain::start(void) {
	Event *clientEvent = d->eventHandler.addEvent();
	
	DeviceManager deviceManager;
	ControllerManager controllerManager;

	ConnectionListener clientListener(L"TelldusClient", clientEvent);
	//TODO: eventlistener

	std::list<ClientCommunicationHandler *> clientCommunicationHandlerList;

	while(!d->stopEvent->isSignaled()) {
		if (!d->eventHandler.waitForAny()) {
			continue;
		}
		if (clientEvent->isSignaled()) {
			//New client connection
			EventData *eventData = clientEvent->takeSignal();
			ConnectionListenerEventData *data = reinterpret_cast<ConnectionListenerEventData*>(eventData);
			if (data) {
				Event *handlerEvent = d->eventHandler.addEvent();
				ClientCommunicationHandler *clientCommunication = new ClientCommunicationHandler(data->socket, handlerEvent, &deviceManager);
				clientCommunication->start();
				clientCommunicationHandlerList.push_back(clientCommunication);
			}
			delete eventData;
		}


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

void TelldusMain::stop(void){
	d->stopEvent->signal();
}
