#include "EventHandler.h"

#include <windows.h>

class EventHandler::PrivateData {
public:
	HANDLE *events;
	int eventCount;
};

EventHandler::EventHandler() {
	d = new PrivateData;
	d->eventCount = 0;
	d->events = new HANDLE[0];
}

EventHandler::~EventHandler(void) {
	delete[] d->events;
	delete d;
}

Event *EventHandler::addEvent() {
/*	EVENT *newArray = new EVENT[d->eventCount+1];
	for (int i = 0; i < d->eventCount; ++i) {
		newArray[i] = d->events[i];
	}
	delete[] d->events;
	d->events = newArray;
	d->events[d->eventCount] = event;
	++d->eventCount;*/
	return 0;
}

void EventHandler::waitForAny() {
	int result = WaitForMultipleObjects(d->eventCount, d->events, FALSE, 3000);
	if (result == WAIT_TIMEOUT) {
		return;
	}
	int eventIndex = result - WAIT_OBJECT_0;
	if (eventIndex >= d->eventCount) {
		return;
	}
	return;

}

/*EVENT EventHandler::createEvent() {
	return CreateEvent(NULL, true, true, NULL);
}*/