#include "EventHandler.h"

class EventHandler::PrivateData {
public:
	EVENT *events;
	int eventCount;
};

EventHandler::EventHandler() {
	d = new PrivateData;
	d->eventCount = 0;
	d->events = new EVENT[0];
}

EventHandler::~EventHandler(void) {
	delete[] d->events;
	delete d;
}

void EventHandler::addEvent(EVENT event) {
	EVENT *newArray = new EVENT[d->eventCount+1];
	for (int i = 0; i < d->eventCount; ++i) {
		newArray[i] = d->events[i];
	}
	delete[] d->events;
	d->events = newArray;
	d->events[d->eventCount] = event;
	++d->eventCount;
}

EVENT EventHandler::waitForAny() {
	int result = WaitForMultipleObjects(d->eventCount, d->events, FALSE, 3000);
	if (result == WAIT_TIMEOUT) {
		return 0;
	}
	int eventIndex = result - WAIT_OBJECT_0;
	if (eventIndex >= d->eventCount) {
		return 0;
	}
	return d->events[eventIndex];

}

EVENT EventHandler::createEvent() {
	return CreateEvent(NULL, true, true, NULL);
}