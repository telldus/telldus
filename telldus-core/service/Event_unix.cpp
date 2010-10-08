#include "Event.h"
#include "EventHandler.h"
#include "Thread.h"

class Event::PrivateData {
public:
	bool signaled;
	EventHandler *handler;
	MUTEX_T mutex;
};

Event::Event(EventHandler *handler) {
	d = new PrivateData;
	d->signaled = false;
	d->handler = handler;
	TelldusCore::Thread::initMutex(&d->mutex);
}

Event::~Event(void) {
	TelldusCore::Thread::destroyMutex(&d->mutex);
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
	d->handler->signal(this);
}

void Event::setSignaled() {
	TelldusCore::MutexLocker locker(&d->mutex);
	d->signaled = true;
}
