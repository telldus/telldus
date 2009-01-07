//
// C++ Implementation: controller
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "Manager.h"
#include "Device.h"

#include "DeviceNexa.h"
#include "DeviceWaveman.h"
#include "DeviceSartano.h"
#include "DeviceIkea.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WINDOWS
#define strcasecmp _stricmp
#endif

Manager *Manager::instance = 0;

Manager::Manager() {
}

Manager::~Manager() {
	// Clean up the device-map
	for (DeviceMap::iterator it = devices.begin(); it != devices.end(); ++it) {
		delete( it->second );
	}
}

/**
 * Get the requested device
 * Note that the Manager keeps ownership of the returned Device
 * and should not be deleted when not in use anymore.
 **/
Device *Manager::getDevice(int intDeviceId){
	Device* dev = NULL;
	
	DeviceMap::iterator iterator = devices.find(intDeviceId);
	if (iterator != devices.end()) {
		return iterator->second;
	}

	try{
		std::string protocol = settings.getProtocol(intDeviceId);
		if (protocol.length() == 0) {
			return NULL;
		}
		int intModel = settings.getModel(intDeviceId);

		//each new brand must be added here
		if (strcasecmp(protocol.c_str(), "arctech") == 0){
			std::string strHouse = settings.getDeviceParameter(intDeviceId, "nexa_house");
			std::string strCode = settings.getDeviceParameter(intDeviceId, "nexa_unit");
			dev = new DeviceNexa(intModel, strHouse, strCode);
	
		} else if (strcasecmp(protocol.c_str(), "Waveman") == 0) {
			std::string strHouse = settings.getDeviceParameter(intDeviceId, "nexa_house");
			std::string strCode = settings.getDeviceParameter(intDeviceId, "nexa_unit");
			dev = new DeviceWaveman(intModel, strHouse, strCode);

		} else if (strcasecmp(protocol.c_str(), "Sartano") == 0) {
			std::string strCode = settings.getDeviceParameter(intDeviceId, "sartano_code");
			dev = new DeviceSartano(intModel, strCode);

		} else if (strcasecmp(protocol.c_str(), "Ikea") == 0) {
			std::string strSystem = settings.getDeviceParameter(intDeviceId, "ikea_system");
			std::string strUnits = settings.getDeviceParameter(intDeviceId, "ikea_units");
			std::string strFade = settings.getDeviceParameter(intDeviceId, "ikea_fade");
			dev = new DeviceIkea(intModel, strSystem, strUnits, strFade);

		} else {
			return NULL;
		}

#ifdef _LINUX
		dev->setDevice( settings.getSetting("deviceNode") );
#endif

	}
	catch(...){
		throw;
	}
	
	devices[intDeviceId] = dev;
	return dev;
}

Manager *Manager::getInstance() {
	if (Manager::instance == 0) {
		Manager::instance = new Manager();
	}
	return Manager::instance;
}

void Manager::close() {
	if (Manager::instance != 0) {
		delete Manager::instance;
	}
}


