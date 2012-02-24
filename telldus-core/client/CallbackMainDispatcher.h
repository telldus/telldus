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

namespace TelldusCore {
	class DeviceChangeEventCallbackData : public EventDataBase {
	public:
		CallbackStruct<TDDeviceChangeEvent> *data;
		int deviceId;
		int eventDeviceChanges;
		int eventChangeType;
	};

	class DeviceEventCallbackData : public EventDataBase {
	public:
		CallbackStruct<TDDeviceEvent> *data;
		int deviceId;
		int deviceState;
		std::string deviceStateValue;
	};

	class RawDeviceEventCallbackData : public EventDataBase {
	public:
		CallbackStruct<TDRawDeviceEvent> *data;
		int controllerId;
		std::string command;
	};

	class SensorEventCallbackData : public EventDataBase {
	public:
		CallbackStruct<TDSensorEvent> *data;
		std::string protocol;
		std::string model;
		int id;
		int dataType;
		std::string value;
		int timestamp;
	};

	class CallbackMainDispatcher  : public Thread
	{
	public:
		CallbackMainDispatcher(void);
		~CallbackMainDispatcher(void);

		EventRef retrieveCallbackEvent();
		
	protected:
		void run();

	private:
		class PrivateData;
		PrivateData *d;
		void cleanupCallbacks(void);
	};
}

#endif //CALLBACKMAINDISPATCHER_H
