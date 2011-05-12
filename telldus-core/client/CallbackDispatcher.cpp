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

TDDeviceEventDispatcher::TDDeviceEventDispatcher(CallbackStruct<TDDeviceEvent> *data, int id, int m, const std::string &strD)
:Thread(), d(data), deviceId(id), method(m), strData(strD)
{
	d->mutex.lock();
	this->start();
}

TDDeviceEventDispatcher::~TDDeviceEventDispatcher() {
	this->wait();
	d->mutex.unlock();
}

void TDDeviceEventDispatcher::run() {
	d->event(deviceId, method, strData.c_str(), d->id, d->context);
}


TDDeviceChangeEventDispatcher::TDDeviceChangeEventDispatcher(CallbackStruct<TDDeviceChangeEvent> *data, int id, int event, int type)
:Thread(), d(data), deviceId(id), changeEvent(event), changeType(type)
{
	d->mutex.lock();
	this->start();
}

TDDeviceChangeEventDispatcher::~TDDeviceChangeEventDispatcher() {
	this->wait();
	d->mutex.unlock();
}

void TDDeviceChangeEventDispatcher::run() {
	d->event(deviceId, changeEvent, changeType, d->id, d->context);
}

TDRawDeviceEventDispatcher::TDRawDeviceEventDispatcher( CallbackStruct<TDRawDeviceEvent> *data, const std::string &strD, int id)
:Thread(), d(data), controllerId(id), strData(strD)
{
	d->mutex.lock();
	this->start();
}

TDRawDeviceEventDispatcher::~TDRawDeviceEventDispatcher() {
	this->wait();
	d->mutex.unlock();
}

void TDRawDeviceEventDispatcher::run() {
	d->event(strData.c_str(), controllerId, d->id, d->context);
}

TDSensorEventDispatcher::TDSensorEventDispatcher( CallbackStruct<TDSensorEvent> *data, const std::string &p, const std::string &m, int id, int type, const std::string &v, int t)
	:Thread(), d(data), protocol(p), model(m), sensorId(id), dataType(type), value(v), timestamp(t)
{
	d->mutex.lock();
	this->start();
}

TDSensorEventDispatcher::~TDSensorEventDispatcher() {
	this->wait();
	d->mutex.unlock();
}

void TDSensorEventDispatcher::run() {
	d->event(protocol.c_str(), model.c_str(), sensorId, dataType, value.c_str(), timestamp, d->id, d->context);
}
