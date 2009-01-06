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
class Device;

typedef std::map<int, Device *> DeviceMap;

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class Manager {
public:
	~Manager();
	
	Device *getDevice(int deviceId);
	
	static Manager *getInstance();
	static void close();
	
private:
	Manager();
	
	Settings settings;
	DeviceMap devices;
	
	static Manager *instance;
};

#endif
