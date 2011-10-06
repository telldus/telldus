/*
 *  CallbackDispatcher.h
 *  telldus-core
 *
 *  Created by Micke Prag on 2010-11-02.
 *  Copyright 2010 Telldus Technologies AB. All rights reserved.
 *
 */


#ifndef CALLBACKDISPATCHER_H
#define CALLBACKDISPATCHER_H

#include "Thread.h"
#include "Mutex.h"
#include "telldus-core.h"

namespace TelldusCore {
	template <typename T> struct CallbackStruct {
		T event;
		int id;
		void *context;
		TelldusCore::Mutex mutex;
	};

	class TDDeviceEventDispatcher : public Thread {
	public:
		TDDeviceEventDispatcher(CallbackStruct<TDDeviceEvent> *data, int deviceId, int method, const std::string &strData);
		virtual ~TDDeviceEventDispatcher();
		bool done() const;
	protected:
		virtual void run();
		bool doneRunning;
	private:
		CallbackStruct<TDDeviceEvent> *d;
		int deviceId, method;
		std::string strData;
	};
	class TDDeviceChangeEventDispatcher : public Thread {
	public:
		TDDeviceChangeEventDispatcher(CallbackStruct<TDDeviceChangeEvent> *data, int deviceId, int changeEvent, int changeType);
		virtual ~TDDeviceChangeEventDispatcher();
		bool done() const;
	protected:
		virtual void run();
		bool doneRunning;
	public:
		CallbackStruct<TDDeviceChangeEvent> *d;
		int deviceId, changeEvent, changeType;
	};
	class TDRawDeviceEventDispatcher : public Thread {
	public:
		TDRawDeviceEventDispatcher( CallbackStruct<TDRawDeviceEvent> *data, const std::string &strData, int controllerId);
		virtual ~TDRawDeviceEventDispatcher();
		bool done() const;
	protected:
		virtual void run();
		bool doneRunning;
	private:
		CallbackStruct<TDRawDeviceEvent> *d;
		int controllerId;
		std::string strData;
	};
	class TDSensorEventDispatcher : public Thread {
	public:
		TDSensorEventDispatcher( CallbackStruct<TDSensorEvent> *data, const std::string &protocol, const std::string &model, int id, int dataType, const std::string &value, int timestamp);
		virtual ~TDSensorEventDispatcher();
		bool done() const;
	protected:
		virtual void run();
		bool doneRunning;
	private:
		CallbackStruct<TDSensorEvent> *d;
		std::string protocol;
		std::string model;
		int sensorId;
		int dataType;
		std::string value;
		int timestamp;
	};
}

#endif //CALLBACKDISPATCHER_H
