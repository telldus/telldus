#include "common/Event.h"
#include "common/EventHandler.h"
#include "common/Thread.h"

using namespace TelldusCore;

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
