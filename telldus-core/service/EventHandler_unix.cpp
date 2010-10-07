#include "EventHandler.h"

class EventHandler::PrivateData {
public:
};

EventHandler::EventHandler() {
	d = new PrivateData;
}

EventHandler::~EventHandler(void) {
	delete d;
}

void EventHandler::addEvent(EVENT event) {
}

EVENT EventHandler::addEvent() {
	EVENT event = EventHandler::createEvent();
	this->addEvent(event);
	return event;
}

EVENT EventHandler::waitForAny() {
}

EVENT EventHandler::createEvent() {
}