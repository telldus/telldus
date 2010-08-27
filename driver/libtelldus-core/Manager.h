//
// C++ Interface: controller
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MANAGER_H
#define MANAGER_H

#include "Settings.h"
#include <map>
#include <list>

namespace TelldusCore {
	class Device;
	class Controller;

	struct CallbackStruct {
		TDDeviceEvent event;
		int id;
		void *context;
	};
	struct RawCallbackStruct {
		TDRawDeviceEvent event;
		int id;
		void *context;
	};

	typedef std::map<int, Device *> DeviceMap;
	typedef std::map<int, Controller *> ControllerMap;
	typedef std::list<CallbackStruct> CallbackList;
	typedef std::list<RawCallbackStruct> RawCallbackList;

	/**
		@author Micke Prag <micke.prag@telldus.se>
	*/
	class Manager {
	public:
		~Manager();
		
		Device *getDevice(int deviceId);
		
		bool deviceLoaded(int deviceId) const;
		
		int getNumberOfDevices(void) const;
		int getDeviceId(int intDeviceIndex) const;
		bool setDeviceProtocol(int intDeviceId, const std::string &strProtocol);
		bool setDeviceName(int intDeviceId, const std::string &strNewName);
		bool setDeviceModel(int intDeviceId, const std::string &strModel);
		bool setDeviceParameter(int intDeviceId, const std::string &strName, const std::string &strValue);
		std::string getDeviceParameter(int intDeviceId, const std::string &strName) const;
		bool setDeviceState( int intDeviceId, int intDeviceState, const std::string &strDeviceStateValue );
		int getDeviceState( int intDeviceId ) const;
		std::string getDeviceStateValue( int intDeviceId ) const;
		
		int addDevice();
		bool removeDevice(int intDeviceId);
		
		static Manager *getInstance();
		static void close();
		
		int registerDeviceEvent( TDDeviceEvent eventFunction, void *context );
		int registerRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context );
		void parseMessage( const std::string &message );
	
	protected:
		void loadAllDevices();
		
	private:
		Manager();
		
		Settings settings;
		DeviceMap devices;
		ControllerMap controllers;
		CallbackList callbacks;
		RawCallbackList rawCallbacks;
		
		int lastCallbackId;
		
		static Manager *instance;
	};
}

#endif
