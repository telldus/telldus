/*
 *  CallbackDispatcher.cpp
 *  telldus-core
 *
 *  Created by Micke Prag on 2010-11-02.
 *  Copyright 2010 Telldus Technologies AB. All rights reserved.
 *
 */

#include "CallbackDispatcher.h"
#include "common.h"

using namespace TelldusCore;

TDDeviceEventDispatcher::TDDeviceEventDispatcher(CallbackStruct<TDDeviceEvent> *data, int id, int m, const std::string &strD)
:Thread(), d(data), deviceId(id), method(m), strData(strD), doneRunning(false)
{
	this->startAndLock(&d->mutex);
}

TDDeviceEventDispatcher::~TDDeviceEventDispatcher() {
	this->wait();
}

bool TDDeviceEventDispatcher::done() const {
	return doneRunning;
}

void TDDeviceEventDispatcher::run() {
	char *str = wrapStdString(strData);
	d->event(deviceId, method, strData.c_str(), d->id, d->context);

	doneRunning = true;
}


TDDeviceChangeEventDispatcher::TDDeviceChangeEventDispatcher(CallbackStruct<TDDeviceChangeEvent> *data, int id, int event, int type)
:Thread(), d(data), deviceId(id), changeEvent(event), changeType(type), doneRunning(false)
{
	this->startAndLock(&d->mutex);
}

TDDeviceChangeEventDispatcher::~TDDeviceChangeEventDispatcher() {
	this->wait();
}

bool TDDeviceChangeEventDispatcher::done() const {
	return doneRunning;
}

void TDDeviceChangeEventDispatcher::run() {
	d->event(deviceId, changeEvent, changeType, d->id, d->context);
	doneRunning = true;
}

TDRawDeviceEventDispatcher::TDRawDeviceEventDispatcher( CallbackStruct<TDRawDeviceEvent> *data, const std::string &strD, int id)
:Thread(), d(data), controllerId(id), strData(strD), doneRunning(false)
{
	this->startAndLock(&d->mutex);
}

TDRawDeviceEventDispatcher::~TDRawDeviceEventDispatcher() {
	this->wait();
}

bool TDRawDeviceEventDispatcher::done() const {
	return doneRunning;
}

void TDRawDeviceEventDispatcher::run() {
	d->event(strData.c_str(), controllerId, d->id, d->context);
	doneRunning = true;
}

TDSensorEventDispatcher::TDSensorEventDispatcher( CallbackStruct<TDSensorEvent> *data, const std::string &p, const std::string &m, int id, int type, const std::string &v, int t)
	:Thread(), d(data), protocol(p), model(m), sensorId(id), dataType(type), value(v), timestamp(t), doneRunning(false)
{
	this->startAndLock(&d->mutex);
}

TDSensorEventDispatcher::~TDSensorEventDispatcher() {
	this->wait();
}

bool TDSensorEventDispatcher::done() const {
	return doneRunning;
}

void TDSensorEventDispatcher::run() {
	d->event(protocol.c_str(), model.c_str(), sensorId, dataType, value.c_str(), timestamp, d->id, d->context);
	doneRunning = true;
}
