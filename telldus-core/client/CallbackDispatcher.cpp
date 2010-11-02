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
