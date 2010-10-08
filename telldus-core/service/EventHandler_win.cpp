#include "EventHandler.h"
#include "Event.h"

#include <windows.h>
#include <list>

class EventHandler::PrivateData {
public:
	HANDLE *eventArray;
	std::list<Event *> eventList;
	int eventCount;
};

EventHandler::EventHandler() {
	d = new PrivateData;
	d->eventCount = 0;
	d->eventArray = new HANDLE[0];
}

EventHandler::~EventHandler(void) {
	delete[] d->eventArray;
	delete d;
}

Event *EventHandler::addEvent() {
	Event *event = new Event(this);
	HANDLE *newArray = new HANDLE[d->eventCount+1];
	for (int i = 0; i < d->eventCount; ++i) {
		newArray[i] = d->eventArray[i];
	}
	delete[] d->eventArray;
	d->eventArray = newArray;
	d->eventArray[d->eventCount] = event->retrieveNative();
	d->eventList.push_back(event);
	++d->eventCount;
	return event;
}

void EventHandler::signal(Event *event) {
}

bool EventHandler::waitForAny() {
	int result = WaitForMultipleObjects(d->eventCount, d->eventArray, FALSE, 3000);
	if (result == WAIT_TIMEOUT) {
		return false;
	}
	int eventIndex = result - WAIT_OBJECT_0;
	if (eventIndex >= d->eventCount) {
		return false;
	}

	return true;

}
