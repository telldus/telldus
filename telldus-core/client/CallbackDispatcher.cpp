/*
 *  CallbackDispatcher.cpp
 *  telldus-core
 *
 *  Created by Micke Prag on 2010-11-02.
 *  Copyright 2010 Telldus Technologies AB. All rights reserved.
 *
 */

#include "CallbackDispatcher.h"

using namespace TelldusCore;

template <typename T> CallbackDispatcher<T>::CallbackDispatcher(const CallbackStruct<T> &data)
:Thread(), d(data)
{
	this->start();
}

template <typename T> CallbackDispatcher<T>::~CallbackDispatcher() {
	this->wait();
}

TDDeviceEventDispatcher::TDDeviceEventDispatcher(const CallbackStruct<TDDeviceEvent> &data, int id, int m, const std::string &strD)
:CallbackDispatcher<TDDeviceEvent>(data), deviceId(id), method(m), strData(strD) {}

void TDDeviceEventDispatcher::run() {
	d.event(deviceId, method, strData.c_str(), d.id, d.context);
}

TDDeviceChangeEventDispatcher::TDDeviceChangeEventDispatcher(const CallbackStruct<TDDeviceChangeEvent> &data, int id, int event, int type)
:CallbackDispatcher<TDDeviceChangeEvent>(data), deviceId(id), changeEvent(event), changeType(type) {}

void TDDeviceChangeEventDispatcher::run() {
	d.event(deviceId, changeEvent, changeType, d.id, d.context);
}

TDRawDeviceEventDispatcher::TDRawDeviceEventDispatcher(const CallbackStruct<TDRawDeviceEvent> &data, const std::string &strD, int id)
:CallbackDispatcher<TDRawDeviceEvent>(data), controllerId(id), strData(strD) {}

void TDRawDeviceEventDispatcher::run() {
	d.event(strData.c_str(), controllerId, d.id, d.context);
}
