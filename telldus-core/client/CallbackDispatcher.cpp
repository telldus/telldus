/*
 *  CallbackDispatcher.cpp
 *  telldus-core
 *
 *  Created by Micke Prag on 2010-11-02.
 *  Copyright 2010 Telldus Technologies AB. All rights reserved.
 *
 */

#include "client/CallbackDispatcher.h"
#include "common/Event.h"
#include "common/EventHandler.h"

namespace TelldusCore {

class TDEventDispatcher::PrivateData {
public:
	EventHandler eventHandler;
	EventRef stopEvent, callbackEvent;
	int id;
	void *func, *context;
};

TDEventDispatcher::TDEventDispatcher(int id, void *func, void *context)
	:Thread() {
	d = new PrivateData;
	d->stopEvent = d->eventHandler.addEvent();
	d->callbackEvent = d->eventHandler.addEvent();
	d->id = id;
	d->func = func;
	d->context = context;
	this->start();
}

TDEventDispatcher::~TDEventDispatcher() {
	d->stopEvent->signal();
	this->wait();
	delete d;
}

int TDEventDispatcher::id() const {
	return d->id;
}

void TDEventDispatcher::queue(EventDataRef eventData) {
	d->callbackEvent->signal(eventData);
}

void TDEventDispatcher::run() {
	while (!d->stopEvent->isSignaled()) {
		d->eventHandler.waitForAny();
		if (d->callbackEvent->isSignaled()) {
			TelldusCore::EventDataRef eventData = d->callbackEvent->takeSignal();
			this->execute(eventData);
		}
	}
}

TDDeviceEventDispatcher::TDDeviceEventDispatcher(int id, void *func, void *context)
	:TDEventDispatcher(id, func, context)
{}

void TDDeviceEventDispatcher::execute(EventDataRef eventData) {
	DeviceEventCallbackData *data = dynamic_cast<DeviceEventCallbackData *>(eventData.get());
	if (!data) {
		return;
	}
	((TDDeviceEvent)d->func)(data->deviceId, data->deviceState, data->deviceStateValue.c_str(), d->id, d->context);
}

CallbackStruct::CallbackType TDDeviceEventDispatcher::type() {
	return CallbackStruct::DeviceEvent;
}

TDDeviceChangeEventDispatcher::TDDeviceChangeEventDispatcher(int id, void *func, void *context)
	:TDEventDispatcher(id, func, context)
{}

void TDDeviceChangeEventDispatcher::execute(EventDataRef eventData) {
	DeviceChangeEventCallbackData *data = dynamic_cast<DeviceChangeEventCallbackData *>(eventData.get());
	if (!data) {
		return;
	}
	((TDDeviceChangeEvent)d->func)(data->deviceId, data->changeEvent, data->changeType, d->id, d->context);
}

CallbackStruct::CallbackType TDDeviceChangeEventDispatcher::type() {
	return CallbackStruct::DeviceChangeEvent;
}

TDRawDeviceEventDispatcher::TDRawDeviceEventDispatcher(int id, void *func, void *context)
	:TDEventDispatcher(id, func, context)
{}

void TDRawDeviceEventDispatcher::execute(EventDataRef eventData) {
	RawDeviceEventCallbackData *data = dynamic_cast<RawDeviceEventCallbackData *>(eventData.get());
	if (!data) {
		return;
	}
	((TDRawDeviceEvent)d->func)(data->data.c_str(), data->controllerId, d->id, d->context);
}

CallbackStruct::CallbackType TDRawDeviceEventDispatcher::type() {
	return CallbackStruct::RawDeviceEvent;
}

TDSensorEventDispatcher::TDSensorEventDispatcher(int id, void *func, void *context)
	:TDEventDispatcher(id, func, context)
{}

void TDSensorEventDispatcher::execute(EventDataRef eventData) {
	SensorEventCallbackData *data = dynamic_cast<SensorEventCallbackData *>(eventData.get());
	if (!data) {
		return;
	}
	((TDSensorEvent)d->func)(data->protocol.c_str(), data->model.c_str(), data->id, data->dataType, data->value.c_str(), data->timestamp, d->id, d->context);
}

CallbackStruct::CallbackType TDSensorEventDispatcher::type() {
	return CallbackStruct::SensorEvent;
}

TDControllerEventDispatcher::TDControllerEventDispatcher(int id, void *func, void *context)
	:TDEventDispatcher(id, func, context)
{}

void TDControllerEventDispatcher::execute(EventDataRef eventData) {
	ControllerEventCallbackData *data = dynamic_cast<ControllerEventCallbackData *>(eventData.get());
	if (!data) {
		return;
	}
	((TDControllerEvent)d->func)(data->controllerId, data->changeEvent, data->changeType, data->newValue.c_str(), d->id, d->context);
}

CallbackStruct::CallbackType TDControllerEventDispatcher::type() {
	return CallbackStruct::ControllerEvent;
}

}  // namespace TelldusCore
