/*
 *  CallbackMainDispatcher.h
 *  telldus-core
 *
 *  Created by Stefan Persson on 2012-02-23.
 *  Copyright 2012 Telldus Technologies AB. All rights reserved.
 *
 */

#ifndef CALLBACKMAINDISPATCHER_H
#define CALLBACKMAINDISPATCHER_H

#include "CallbackDispatcher.h"
#include "Thread.h"
#include "Event.h"
#include "EventHandler.h"

class DeviceChangeEventCallbackData : public TelldusCore::EventDataBase {
public:
	TelldusCore::CallbackStruct<TDDeviceChangeEvent> *data;
	int deviceId;
	int eventDeviceChanges;
	int eventChangeType;
};

class DeviceEventCallbackData : public TelldusCore::EventDataBase {
public:
	TelldusCore::CallbackStruct<TDDeviceEvent> *data;
	int deviceId;
	int deviceState;
	std::string deviceStateValue;
};

class RawDeviceEventCallbackData : public TelldusCore::EventDataBase {
public:
	TelldusCore::CallbackStruct<TDRawDeviceEvent> *data;
	int controllerId;
	std::string command;
};

class SensorEventCallbackData : public TelldusCore::EventDataBase {
public:
	TelldusCore::CallbackStruct<TDSensorEvent> *data;
	std::string protocol;
	std::string model;
	int id;
	int dataType;
	std::string value;
	int timestamp;
};

class CallbackMainDispatcher  : public TelldusCore::Thread
{
public:
	CallbackMainDispatcher(void);
	~CallbackMainDispatcher(void);

	TelldusCore::EventRef retrieveCallbackEvent();
	
protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
	void cleanupCallbacks(void);
};

#endif //CALLBACKMAINDISPATCHER_H
