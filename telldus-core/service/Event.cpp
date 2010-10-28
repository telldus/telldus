#include "Event.h"
#include "EventHandler.h"
#include "Mutex.h"

#include <list>

EventData::~EventData() {
}

bool EventData::isValid() const {
	return false;
};

bool EventDataBase::isValid() const {
	return true;
};

class EventBase::PrivateData {
public:
	TelldusCore::Mutex mutex;
	EventHandler *handler;
	std::list<EventDataRef> eventDataList;
};

EventBase::EventBase(EventHandler *handler) {
	d = new PrivateData;
	d->handler = handler;
}

EventBase::~EventBase(void) {
	delete d;
}

void EventBase::clearHandler() {
	TelldusCore::MutexLocker locker(&d->mutex);
	d->handler = 0;
}

void EventBase::popSignal() {
	this->takeSignal();
}

EventHandler *EventBase::handler() const {
	return d->handler;
}

bool EventBase::isSignaled() {
	TelldusCore::MutexLocker locker(&d->mutex);
	return (d->eventDataList.size() > 0);
}

void EventBase::signal() {
	signal(new EventData());
}

void EventBase::signal(EventData *eventData) {
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		d->eventDataList.push_back(EventDataRef(eventData));
	}
	sendSignal();
}

EventDataRef EventBase::takeSignal() {
	TelldusCore::MutexLocker locker(&d->mutex);
	if (d->eventDataList.size() == 0) {
		return EventDataRef(new EventData());
	}
	EventDataRef data = d->eventDataList.front();
	d->eventDataList.pop_front();
	if (d->eventDataList.size() == 0) {
		this->clearSignal();
	}
	return data;
}
