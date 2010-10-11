#include "Event.h"
#include "EventHandler.h"
#include "Thread.h"

class Event::PrivateData {
public:
};

Event::Event(EventHandler *handler) {
	d = new PrivateData;
}

Event::~Event(void) {
	delete d;
}

void Event::sendSignal() {
	EventHandler *handler = this->handler();
	if (handler) {
		handler->signal(this);
	}
}
