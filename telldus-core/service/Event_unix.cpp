#include "Event.h"
#include "EventHandler.h"

class Event::PrivateData {
public:
	bool signaled;
	EventHandler *handler;
};

Event::Event(EventHandler *handler) {
	d = new PrivateData;
	d->signaled = false;
}

Event::~Event(void) {
	delete d;
}

bool Event::isSignaled() {
	return d->signaled;
}

void Event::signal() {
	d->handler->signal(this);
}

void Event::setSignaled() {
	d->signaled = true;
}