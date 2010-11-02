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
	
	template <typename T> class CallbackDispatcher: public Thread {
	public:
		CallbackDispatcher(const CallbackStruct<T> &data);
		virtual ~CallbackDispatcher();
	protected:
		CallbackStruct<T> d;
	};
	
	class TDDeviceEventDispatcher : public CallbackDispatcher<TDDeviceEvent> {
	public:
		TDDeviceEventDispatcher(const CallbackStruct<TDDeviceEvent> &data, int deviceId, int method, const std::string &strData);
	protected:
		virtual void run();
	private:
		int deviceId, method;
		std::string strData;
	};
	class TDDeviceChangeEventDispatcher : public CallbackDispatcher<TDDeviceChangeEvent> {
	public:
		TDDeviceChangeEventDispatcher(const CallbackStruct<TDDeviceChangeEvent> &data, int deviceId, int changeEvent, int changeType);
	protected:
		virtual void run();
	private:
		int deviceId, changeEvent, changeType;
	};
	class TDRawDeviceEventDispatcher : public CallbackDispatcher<TDRawDeviceEvent> {
	public:
		TDRawDeviceEventDispatcher(const CallbackStruct<TDRawDeviceEvent> &data, const std::string &strData, int controllerId);
	protected:
		virtual void run();
	private:
		int controllerId;
		std::string strData;
	};
}

#endif //CALLBACKDISPATCHER_H
