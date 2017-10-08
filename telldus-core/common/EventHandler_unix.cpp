//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <list>
#include "common/EventHandler.h"
#include "common/Event.h"
#include "common/Mutex.h"
#include "common/Thread.h"

namespace TelldusCore {

class EventHandler::PrivateData {
public:
	pthread_cond_t event;
	pthread_mutex_t mutex;
	std::list<EventRef> eventList;
	TelldusCore::Mutex listMutex;
	bool isSignalled;
};

EventHandler::EventHandler() {
	d = new PrivateData;
	pthread_cond_init(&d->event, NULL);
	pthread_cond_init(&d->event, NULL);
	pthread_mutex_init(&d->mutex, NULL);
	d->isSignalled = false;
}

EventHandler::~EventHandler(void) {
	pthread_mutex_destroy(&d->mutex);
	pthread_cond_destroy(&d->event);

	std::list<EventRef>::const_iterator it = d->eventList.begin();
	for(; it != d->eventList.end(); ++it) {
		// We clear the handler if someone else still has a reference to the event
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

bool EventHandler::listIsSignalled() {
	TelldusCore::MutexLocker locker(&d->listMutex);

	std::list<EventRef>::const_iterator it = d->eventList.begin();
	for(; it != d->eventList.end(); ++it) {
		if((*it)->isSignaled()) {
			return true;
		}
	}
	return false;
}

void EventHandler::signal(Event *event) {
	pthread_mutex_lock(&d->mutex);
	d->isSignalled = true;
	// event->setSignaled();
	pthread_cond_signal(&d->event);
	pthread_mutex_unlock(&d->mutex);
}

bool EventHandler::waitForAny() {
	pthread_mutex_lock(&d->mutex);
	int ret;
	while (!d->isSignalled) {
		timeval now;
		gettimeofday(&now, NULL);
		suseconds_t abstime_ns_large = now.tv_usec*1000 + 60000000000;  // add 60 seconds wait (5 seconds before)?
		timespec abstime = { now.tv_sec + (abstime_ns_large / 1000000000), abstime_ns_large % 1000000000 };
		ret = pthread_cond_timedwait(&d->event, &d->mutex, &abstime);
		if (ret == ETIMEDOUT) {
			continue;
		}
	}
	if (!listIsSignalled()) {
		d->isSignalled = false;
	}
	pthread_mutex_unlock(&d->mutex);
	return listIsSignalled();
}

}  // namespace TelldusCore
