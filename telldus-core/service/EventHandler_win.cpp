#include "EventHandler.h"
#include "Event.h"
#include "Mutex.h"

#include <windows.h>
#include <list>

class EventHandler::PrivateData {
public:
	HANDLE *eventArray;
	Event **eventObjectArray;
	TelldusCore::Mutex mutex;
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

	TelldusCore::MutexLocker locker(&d->mutex);

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

bool EventHandler::removeEvent(EventBase *event) {
	TelldusCore::MutexLocker locker(&d->mutex);
	HANDLE *newArray = new HANDLE[d->eventCount-1];
	Event **newObjectArray = new Event*[d->eventCount-1];
	int index = 0;
	int i = 0;
	for (; i < d->eventCount; ++i) {
		if ( d->eventObjectArray[i] == event ) {
			continue;
		}
		newArray[index] = d->eventArray[i];
		newObjectArray[index] = d->eventObjectArray[i];
		++index;
	}
	delete[] d->eventArray;
	delete[] d->eventObjectArray;
	d->eventArray = newArray;
	d->eventObjectArray = newObjectArray;
	--d->eventCount;
	return (i != index);
}

void EventHandler::signal(Event *) {
}

bool EventHandler::waitForAny() {
	int result = WaitForMultipleObjects(d->eventCount, d->eventArray, FALSE, INFINITE);

	TelldusCore::MutexLocker locker(&d->mutex);
	if (result == WAIT_TIMEOUT) {
		return false;
	}
	int eventIndex = result - WAIT_OBJECT_0;
	if (eventIndex >= d->eventCount) {
		return false;
	}
	return true;
}
