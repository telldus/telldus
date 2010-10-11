#include "EventHandler.h"
#include "Event.h"
#include "Thread.h"
#include <pthread.h>
#include <stdio.h>

class EventHandler::PrivateData {
public:
	pthread_cond_t event;
	pthread_mutex_t mutex;
	bool hasEvent;
};

EventHandler::EventHandler() {
	d = new PrivateData;
	pthread_cond_init(&d->event, NULL);
	pthread_mutex_init(&d->mutex, NULL);
}

EventHandler::~EventHandler(void) {
	pthread_mutex_destroy(&d->mutex);
	pthread_cond_destroy(&d->event);
	delete d;
}

Event *EventHandler::addEvent() {
	Event *event = new Event(this);
	return event;
}

void EventHandler::signal(Event *event) {
	pthread_mutex_lock(&d->mutex);
	//event->setSignaled();
	d->hasEvent = true;
	pthread_cond_signal(&d->event);
	pthread_mutex_unlock(&d->mutex);
}

bool EventHandler::waitForAny() {
	pthread_mutex_lock(&d->mutex);
	while(!d->hasEvent) {
		pthread_cond_wait(&d->event, &d->mutex);
	}
	d->hasEvent = false;
	pthread_mutex_unlock(&d->mutex);

	return true;
}
