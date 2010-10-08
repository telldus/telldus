#include "Event.h"
#include "EventHandler.h"

class Event::PrivateData {
public:
	bool signaled;
	EventHandler *handler;
	EVENT_T event;
};

Event::Event(EventHandler *handler) {
	d = new PrivateData;
	d->signaled = false;
	d->event = CreateEvent(NULL, true, true, NULL);
}

Event::~Event(void) {
	delete d;
}

bool Event::isSignaled() {
	return d->signaled;
}

void Event::signal() {
	//d->handler->signal(this);
}

void Event::setSignaled() {
	d->signaled = true;
}

EVENT_T Event::retrieveNative() {
	return d->event;
}