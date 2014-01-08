/*
 *  CallbackMainDispatcher.cpp
 *  telldus-core
 *
 *  Created by Stefan Persson on 2012-02-23.
 *  Copyright 2012 Telldus Technologies AB. All rights reserved.
 *
 */

#include "client/CallbackMainDispatcher.h"

#include <list>

namespace TelldusCore {

typedef std::list<TelldusCore::TDEventDispatcher *> CallbackList;

class CallbackMainDispatcher::PrivateData {
public:
	Mutex mutex;
	CallbackList callbackList;
	int lastCallbackId;
};

CallbackMainDispatcher::CallbackMainDispatcher() {
	d = new PrivateData;
	d->lastCallbackId = 0;
}

CallbackMainDispatcher::~CallbackMainDispatcher(void) {
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(CallbackList::iterator it = d->callbackList.begin(); it != d->callbackList.end(); ++it) {
			delete (*it);
		}
	}
	delete d;
}

void CallbackMainDispatcher::execute(CallbackStruct::CallbackType type, EventData *eventData) {
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		EventDataRef eventDataRef(eventData);
		for(CallbackList::iterator callback_it = d->callbackList.begin(); callback_it != d->callbackList.end(); ++callback_it) {
			if ( (*callback_it)->type() != type ) {
				continue;
			}
			(*callback_it)->queue(eventDataRef);
		}
	}
}

int CallbackMainDispatcher::registerCallback(CallbackStruct::CallbackType type, void *eventFunction, void *context) {
	TelldusCore::MutexLocker locker(&d->mutex);
	int id = ++d->lastCallbackId;
	TelldusCore::TDEventDispatcher *callback;
	if (type == CallbackStruct::DeviceEvent) {
		callback = new TelldusCore::TDDeviceEventDispatcher(id, eventFunction, context);
	} else if (type == CallbackStruct::DeviceChangeEvent) {
		callback = new TelldusCore::TDDeviceChangeEventDispatcher(id, eventFunction, context);
	} else if (type == CallbackStruct::RawDeviceEvent) {
		callback = new TelldusCore::TDRawDeviceEventDispatcher(id, eventFunction, context);
	} else if (type == CallbackStruct::SensorEvent) {
		callback = new TelldusCore::TDSensorEventDispatcher(id, eventFunction, context);
	} else if (type == CallbackStruct::ControllerEvent) {
		callback = new TelldusCore::TDControllerEventDispatcher(id, eventFunction, context);
	} else {
		return -1;
	}
	d->callbackList.push_back(callback);
	return id;
}

int CallbackMainDispatcher::unregisterCallback(int callbackId) {
	CallbackList newEventList;
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(CallbackList::iterator callback_it = d->callbackList.begin(); callback_it != d->callbackList.end(); ++callback_it) {
			if ( (*callback_it)->id() != callbackId ) {
				continue;
			}
			newEventList.splice(newEventList.begin(), d->callbackList, callback_it);
			break;
		}
	}
	if (newEventList.size()) {
		CallbackList::iterator it = newEventList.begin();
		delete (*it);
		newEventList.erase(it);
		return TELLSTICK_SUCCESS;
	}
	return TELLSTICK_ERROR_NOT_FOUND;
}

}  // namespace TelldusCore
