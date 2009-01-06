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
		char* protocol = settings.getProtocol(intDeviceId);
		if (protocol == NULL) {
			return NULL;
		}

		//each new brand must be added here
		if (strcasecmp(protocol, "arctech") == 0){
			char *strHouse = settings.getDeviceParameter(intDeviceId, "nexa_house");
			char *strCode = settings.getDeviceParameter(intDeviceId, "nexa_unit");
			dev = new DeviceNexa(strHouse, strCode);
			free(strHouse);
			free(strCode);
		
		} else if (strcasecmp(protocol, "Waveman") == 0) {
			char *strHouse = settings.getDeviceParameter(intDeviceId, "nexa_house");
			char *strCode = settings.getDeviceParameter(intDeviceId, "nexa_unit");
			dev = new DeviceWaveman(strHouse, strCode);
			free(strHouse);
			free(strCode);

		} else if (strcasecmp(protocol, "Sartano") == 0) {
			char *strCode = settings.getDeviceParameter(intDeviceId, "sartano_code");
			dev = new DeviceSartano(strCode);
			free(strCode);

		} else if (strcasecmp(protocol, "Ikea") == 0) {
			char *strSystem = settings.getDeviceParameter(intDeviceId, "ikea_system");
			char *strUnits = settings.getDeviceParameter(intDeviceId, "ikea_units");
			char *strFade = settings.getDeviceParameter(intDeviceId, "ikea_fade");
			dev = new DeviceIkea(strSystem, strUnits, strFade);
			free(strSystem);
			free(strUnits);
			free(strFade);

		} else {
			free(protocol);
			return NULL;
		}
		free(protocol);

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


