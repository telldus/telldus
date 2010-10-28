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
	std::list<EventRef> eventList;
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

	std::list<EventRef>::const_iterator it = d->eventList.begin();
	for(; it != d->eventList.end(); ++it) {
		//We clear the handler if someone else still has a reference to the event
		(*it)->clearHandler();
	}

	delete d;
}

EventRef EventHandler::addEvent() {
	EventRef event(new Event(this));
	TelldusCore::MutexLocker locker(&d->listMutex);
	d->eventList.push_back(event);
	return event;
}

bool EventHandler::listIsSignalled(){
	TelldusCore::MutexLocker locker(&d->listMutex);

	std::list<EventRef>::const_iterator it = d->eventList.begin();
	for(; it != d->eventList.end(); ++it) {
		if((*it)->isSignaled()){
			return true;
		}
	}
	return false;
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
