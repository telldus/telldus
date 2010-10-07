#include "EventHandler.h"

class EventHandler::PrivateData {
public:
	EVENT event;
};

EventHandler::EventHandler()
{
	d = new PrivateData;

}

EventHandler::~EventHandler(void)
{
	delete d;
}

void EventHandler::addEvent(EVENT event) {
	d->event = event;
}

int EventHandler::waitForAny() {
	int result = WaitForSingleObject(d->event, 3000);
	if (result == WAIT_TIMEOUT) {
		return 0;
	}
	return 1;

}

EVENT EventHandler::createEvent() {
	return CreateEvent(NULL, true, true, NULL);
}