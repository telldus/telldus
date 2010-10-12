#include "Event.h"
#include "EventHandler.h"
#include "Thread.h"

class Event::PrivateData {
public:
};

Event::Event(EventHandler *handler)
	:EventBase(handler){
	d = new PrivateData;
}

Event::~Event(void) {
	delete d;
}

void Event::clearSignal() {
}

void Event::sendSignal() {
	EventHandler *handler = this->handler();
	if (handler) {
		handler->signal(this);
	}
}
