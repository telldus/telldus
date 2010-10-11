#include "Event.h"
#include "EventHandler.h"

#include <list>

bool EventData::isValid() const {
	return false;
};

bool EventDataBase::isValid() const {
	return true;
};

class EventBase::PrivateData {
public:
	MUTEX_T mutex;
	EventHandler *handler;
	std::list<EventData *> eventDataList;
};

EventBase::EventBase(EventHandler *handler) {
	d = new PrivateData;
	d->handler = handler;
	TelldusCore::Thread::initMutex(&d->mutex);
}

EventBase::~EventBase(void) {
	if (d->handler) {
		d->handler->removeEvent(this);
	}
	//If we have signals left, make sure we delete the data
	std::list<EventData *>::const_iterator it = d->eventDataList.begin();
	for(; it != d->eventDataList.end(); ++it) {
		delete(*it);
	}
	TelldusCore::Thread::destroyMutex(&d->mutex);
	delete d;
}

void EventBase::clearHandler() {
	TelldusCore::MutexLocker locker(&d->mutex);
	d->handler = 0;
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
		d->eventDataList.push_back(eventData);
	}
	sendSignal();
}

EventData *EventBase::takeSignal() {
	TelldusCore::MutexLocker locker(&d->mutex);
	if (d->eventDataList.size() == 0) {
		return 0;
	}
	EventData *data = d->eventDataList.front();
	d->eventDataList.pop_front();
	if (d->eventDataList.size() == 0) {
		this->clearSignal();
	}
	if (!data->isValid()) {
		delete data;
		return 0;
	}
	return data;
}