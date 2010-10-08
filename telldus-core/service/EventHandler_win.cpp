#include "EventHandler.h"
#include "Event.h"

#include <windows.h>
#include <list>

class EventHandler::PrivateData {
public:
	HANDLE *eventArray;
	Event **eventObjectArray;
	int eventCount;
};

EventHandler::EventHandler() {
	d = new PrivateData;
	d->eventCount = 0;
	d->eventArray = new HANDLE[0];
	d->eventObjectArray = new Event*[0];
}

EventHandler::~EventHandler(void) {
	delete[] d->eventObjectArray;
	delete[] d->eventArray;
	delete d;
}

Event *EventHandler::addEvent() {
	Event *event = new Event(this);

	HANDLE *newArray = new HANDLE[d->eventCount+1];
	Event **newObjectArray = new Event*[d->eventCount+1];
	for (int i = 0; i < d->eventCount; ++i) {
		newArray[i] = d->eventArray[i];
		newObjectArray[i] = d->eventObjectArray[i];
	}
	delete[] d->eventArray;
	delete[] d->eventObjectArray;
	d->eventArray = newArray;
	d->eventObjectArray = newObjectArray;
	d->eventArray[d->eventCount] = event->retrieveNative();
	d->eventObjectArray[d->eventCount] = event;
	++d->eventCount;
	return event;
}

void EventHandler::signal(Event *event) {
	event->signal();
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
	d->eventObjectArray[eventIndex]->setSignaled();
	return true;

}
