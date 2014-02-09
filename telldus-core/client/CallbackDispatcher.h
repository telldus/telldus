/*
 *  CallbackDispatcher.h
 *  telldus-core
 *
 *  Created by Micke Prag on 2010-11-02.
 *  Copyright 2010 Telldus Technologies AB. All rights reserved.
 *
 */

#ifndef TELLDUS_CORE_CLIENT_CALLBACKDISPATCHER_H_
#define TELLDUS_CORE_CLIENT_CALLBACKDISPATCHER_H_

#include <string>
#include "common/common.h"
#include "common/Event.h"
#include "common/Thread.h"
#include "common/Mutex.h"
#include "client/telldus-core.h"

namespace TelldusCore {

	struct CallbackStruct {
		enum CallbackType { DeviceEvent, DeviceChangeEvent, RawDeviceEvent, SensorEvent, ControllerEvent };
		CallbackType type;
		void *event;
		int id;
		void *context;
		TelldusCore::Mutex mutex;
	};

	class CallbackData: public EventDataBase {
	public:
		explicit CallbackData(CallbackStruct::CallbackType t) : EventDataBase(), type(t) {}
		CallbackStruct::CallbackType type;
	};

	class DeviceEventCallbackData : public CallbackData {
	public:
		DeviceEventCallbackData() : CallbackData(CallbackStruct::DeviceEvent) {}
		int deviceId;
		int deviceState;
		std::string deviceStateValue;
	};
	class DeviceChangeEventCallbackData : public CallbackData {
	public:
		DeviceChangeEventCallbackData() : CallbackData(CallbackStruct::DeviceChangeEvent) {}
		int deviceId;
		int changeEvent;
		int changeType;
	};

	class RawDeviceEventCallbackData : public CallbackData {
	public:
		RawDeviceEventCallbackData() : CallbackData(CallbackStruct::RawDeviceEvent) {}
		std::string data;
		int controllerId;
	};

	class SensorEventCallbackData : public CallbackData {
	public:
		SensorEventCallbackData() : CallbackData(CallbackStruct::SensorEvent) {}
		std::string protocol;
		std::string model;
		int id;
		int dataType;
		std::string value;
		int timestamp;
	};
	class ControllerEventCallbackData : public CallbackData {
	public:
		ControllerEventCallbackData() : CallbackData(CallbackStruct::ControllerEvent) {}
		int controllerId;
		int changeEvent;
		int changeType;
		std::string newValue;
	};

	class TDEventDispatcher : public Thread {
	public:
		TDEventDispatcher(int id, void *func, void *context);
		virtual ~TDEventDispatcher();
		int id() const;
		void queue(EventDataRef eventData);
		virtual CallbackStruct::CallbackType type() = 0;
	protected:
		class PrivateData;
		PrivateData *d;

		virtual void run();
		virtual void execute(EventDataRef eventData) = 0;
	};
	class TDDeviceEventDispatcher : public TDEventDispatcher {
	public:
		TDDeviceEventDispatcher(int id, void *func, void *context);
		virtual CallbackStruct::CallbackType type();
	protected:
		virtual void execute(EventDataRef eventData);
	};
	class TDDeviceChangeEventDispatcher : public TDEventDispatcher {
	public:
		TDDeviceChangeEventDispatcher(int id, void *func, void *context);
		virtual CallbackStruct::CallbackType type();
	protected:
		virtual void execute(EventDataRef eventData);
	};
	class TDRawDeviceEventDispatcher : public TDEventDispatcher {
	public:
		TDRawDeviceEventDispatcher(int id, void *func, void *context);
		virtual CallbackStruct::CallbackType type();
	protected:
		virtual void execute(EventDataRef eventData);
	};
	class TDSensorEventDispatcher : public TDEventDispatcher {
	public:
		TDSensorEventDispatcher(int id, void *func, void *context);
		virtual CallbackStruct::CallbackType type();
	protected:
		virtual void execute(EventDataRef eventData);
	};
	class TDControllerEventDispatcher : public TDEventDispatcher {
	public:
		TDControllerEventDispatcher(int id, void *func, void *context);
		virtual CallbackStruct::CallbackType type();
	protected:
		virtual void execute(EventDataRef eventData);
	};
}

#endif  // TELLDUS_CORE_CLIENT_CALLBACKDISPATCHER_H_
