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

#include <QObject>
#include "Settings.h"
#include <map>

namespace TelldusCore {
	class Device;
	class Controller;

	struct CallbackStruct {
		deviceEvent event;
		void *context;
	};

	typedef std::map<int, Device *> DeviceMap;
	typedef std::map<int, Controller *> ControllerMap;
	typedef std::list<CallbackStruct> CallbackList;

	/**
		@author Micke Prag <micke.prag@telldus.se>
	*/
	class Manager {
	public:
		~Manager();
		
		Device *getDevice(int deviceId);
		
		bool deviceLoaded(int deviceId) const;
		
		bool setProtocol(int intDeviceId, const std::string &strProtocol);
// 		bool setName(int intDeviceId, const std::string &strNewName);
		bool setModel(int intDeviceId, int intModel);
// 		bool setDeviceParameter(int intDeviceId, const std::string &strName, const std::string &strValue);
		
		static Manager *getInstance();
		static void close();
		
		void registerDeviceEvent( deviceEvent eventFunction, void *context );
		void parseMessage( const std::string &message );
		
	private:
		Manager();
		
		Settings settings;
		DeviceMap devices;
		ControllerMap controllers;
		CallbackList callbacks;
		
		static Manager *instance;
	};
}

#endif
