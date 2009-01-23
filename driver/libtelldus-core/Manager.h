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

namespace TelldusCore {
	class Device;

	typedef std::map<int, Device *> DeviceMap;

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
		
	private:
		Manager();
		
		Settings settings;
		DeviceMap devices;
		
		static Manager *instance;
	};
}

#endif
