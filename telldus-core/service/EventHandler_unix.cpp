#include "EventHandler.h"
#include "Event.h"
#include "Mutex.h"
#include "Thread.h"
#include <list>
#include <pthread.h>
#include <stdio.h>

class EventHandler::PrivateData {
public:
	pthread_cond_t event;
	pthread_mutex_t mutex;
	std::list<Event *> eventList;
	TelldusCore::Mutex listMutex;
};

EventHandler::EventHandler() {
	d = new PrivateData;
	pthread_cond_init(&d->event, NULL);
	pthread_mutex_init(&d->mutex, NULL);
}

EventHandler::~EventHandler(void) {
	pthread_mutex_destroy(&d->mutex);
	pthread_cond_destroy(&d->event);
	
	std::list<Event *>::const_iterator it = d->eventList.begin();
	for(; it != d->eventList.end(); ++it) {
		delete(*it);
	}
	
	delete d;
}

Event *EventHandler::addEvent() {
	Event *event = new Event(this);
	TelldusCore::MutexLocker locker(&d->listMutex);
	d->eventList.push_back(event);
	return event;
}

bool EventHandler::listIsSignalled(){
	TelldusCore::MutexLocker locker(&d->listMutex);
	
	std::list<Event *>::const_iterator it = d->eventList.begin();
	for(; it != d->eventList.end(); ++it) {
		if((*it)->isSignaled()){
			return true;
		}
	}
	return false;
}

bool EventHandler::removeEvent(EventBase *event) {
	TelldusCore::MutexLocker locker(&d->listMutex);
	d->eventList.remove((Event*)event);
	return true;
}

void EventHandler::signal(Event *event) {
	pthread_mutex_lock(&d->mutex);
	//event->setSignaled();
	pthread_cond_signal(&d->event);
	pthread_mutex_unlock(&d->mutex);
}

bool EventHandler::waitForAny() {
	pthread_mutex_lock(&d->mutex);
	while(!listIsSignalled()) {
		pthread_cond_wait(&d->event, &d->mutex);
	}
	pthread_mutex_unlock(&d->mutex);

	return true;
}
