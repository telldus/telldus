#include "EventUpdateManager.h"

#include "Socket.h"
#include "EventHandler.h"

#include <list>

typedef std::list<TelldusCore::Socket *> SocketList;

class EventUpdateManager::PrivateData {
public:
	EventHandler eventHandler;
	Event *stopEvent, *updateEvent, *clientConnectEvent;
	SocketList clients;
};

EventUpdateManager::EventUpdateManager()
	:Thread()
{
	d = new PrivateData;
	d->stopEvent = d->eventHandler.addEvent();
	d->updateEvent = d->eventHandler.addEvent();
	d->clientConnectEvent = d->eventHandler.addEvent();
}

EventUpdateManager::~EventUpdateManager(void) {
	d->stopEvent->signal();
	wait();
	delete d->stopEvent;
	delete d->updateEvent;
	delete d->clientConnectEvent;
	
	for (SocketList::iterator it = d->clients.begin(); it != d->clients.end(); ++it) {
		delete(*it);
	}
		
	delete d;
}

Event *EventUpdateManager::retrieveUpdateEvent(){

	return d->updateEvent;
}

Event *EventUpdateManager::retrieveClientConnectEvent(){
	return d->clientConnectEvent;
}

void EventUpdateManager::run(){

	while(!d->stopEvent->isSignaled()){
		if (!d->eventHandler.waitForAny()) {
			continue;
		}

		if(d->clientConnectEvent->isSignaled()){
			//lägg till ny klient
		}
		else if(d->updateEvent->isSignaled()){
			//event har inträffat
			//datan innehåller informationen om eventet som har skickats
			//Finns i eventDatan: int eventDeviceChanges, int eventChangeType, int eventMethod, int deviceType, int deviceId
			EventData *eventData = d->updateEvent->takeSignal();
			EventUpdateData *data = reinterpret_cast<EventUpdateData*>(eventData);
			
			sendMessageToClients(data);	
		}
	}
}

void EventUpdateManager::sendMessageToClients(EventUpdateData *data){
	/*
	for(){
		if(isalive){
			
			it++;
		}
		else{
			//ta bort
			delete *it;
			it = ngt.erase(it);
		}
	}
	*/
}