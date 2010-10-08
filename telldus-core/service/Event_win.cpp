#include "Event.h"
#include "EventHandler.h"
#include "Thread.h"

class Event::PrivateData {
public:
	bool signaled;
	EventHandler *handler;
	EVENT_T event;
	MUTEX_T mutex;
};

Event::Event(EventHandler *handler) {
	d = new PrivateData;
	d->signaled = false;
	d->handler = handler;
	d->event = CreateEvent(NULL, true, false, NULL);
	TelldusCore::Thread::initMutex(&d->mutex);
}

Event::~Event(void) {
	if (d->handler) {
		d->handler->removeEvent(this);
	}
	TelldusCore::Thread::destroyMutex(&d->mutex);
	CloseHandle(d->event);
	delete d;
}

void Event::clearSignal() {
	TelldusCore::MutexLocker locker(&d->mutex);
	d->signaled = false;
}

bool Event::isSignaled() {
	TelldusCore::MutexLocker locker(&d->mutex);
	return d->signaled;
}

void Event::signal() {
	SetEvent(d->event);
}

void Event::clearHandler() {
	TelldusCore::MutexLocker locker(&d->mutex);
	d->handler = 0;
}

void Event::setSignaled() {
	TelldusCore::MutexLocker locker(&d->mutex);
	d->signaled = true;
}

EVENT_T Event::retrieveNative() {
	TelldusCore::MutexLocker locker(&d->mutex);
	return d->event;
}
