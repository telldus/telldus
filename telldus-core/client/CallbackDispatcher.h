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
#include "telldus-core.h"

namespace TelldusCore {
	template <typename T> struct CallbackStruct {
		T event;
		int id;
		void *context;
	};
	
	class TDDeviceEventDispatcher : public Thread {
	public:
		TDDeviceEventDispatcher(const CallbackStruct<TDDeviceEvent> &data, int deviceId, int method, const std::string &strData);
		virtual ~TDDeviceEventDispatcher();
	protected:
		virtual void run();
	private:
		CallbackStruct<TDDeviceEvent> d;
		int deviceId, method;
		std::string strData;
	};
	class TDDeviceChangeEventDispatcher : public Thread {
	public:
		TDDeviceChangeEventDispatcher(const CallbackStruct<TDDeviceChangeEvent> &data, int deviceId, int changeEvent, int changeType);
		virtual ~TDDeviceChangeEventDispatcher();
	protected:
		virtual void run();
	private:
		CallbackStruct<TDDeviceChangeEvent> d;
		int deviceId, changeEvent, changeType;
	};
	class TDRawDeviceEventDispatcher : public Thread {
	public:
		TDRawDeviceEventDispatcher(const CallbackStruct<TDRawDeviceEvent> &data, const std::string &strData, int controllerId);
		virtual ~TDRawDeviceEventDispatcher();
	protected:
		virtual void run();
	private:
		CallbackStruct<TDRawDeviceEvent> d;
		int controllerId;
		std::string strData;
	};
}

#endif //CALLBACKDISPATCHER_H
