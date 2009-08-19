#include <stdlib.h>

#include "DeviceGroup.h"
#include "Manager.h"

using namespace TelldusCore;

/*
* Constructor
*/
DeviceGroup::DeviceGroup(int id, const std::string &model, const std::string &name)
	:Device(id, model, name)
{
}

/*
* Destructor
*/
DeviceGroup::~DeviceGroup(void) {
}

bool DeviceGroup::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("devices") == 0) {
		return setDevices(strValue);
	}
	return false;
}

bool DeviceGroup::setDevices(const std::string &newDevices) {
/*	if (strDevices.length() > 0) {
		char *tempDevices = new char[strDevices.size()+1];
	#ifdef _WINDOWS
		strcpy_s(tempDevices, strDevices.size()+1, strDevices.c_str());
	#else
		strcpy(tempDevices, strDevices.c_str());
	#endif
	
		Manager *manager = Manager::getInstance();
		char *strToken = strtok(tempDevices, ",");
		do {
		int intDevice = atoi(strToken);
		Device *device = manager->getDevice(intDevice);
		if (device != NULL) {
		deviceList.push_back( device );
	}
	} while ( (strToken = strtok(NULL, ",")) != NULL );
	
		free(tempDevices);
	}*/
	return true;
}

/*
* Turn on this device
*/
int DeviceGroup::turnOn(void) {
	int retVal = TELLSTICK_ERROR_UNKNOWN;
	
	for (DeviceList::const_iterator it = deviceList.begin(); it != deviceList.end(); ++it) {
		int methods = (*it)->methods();
		if (methods & TELLSTICK_TURNON) {
			int success = (*it)->switchState( TELLSTICK_TURNON );
			if (retVal != TELLSTICK_SUCCESS) {
				retVal = success;
			}
		}
	}
	return retVal;
}

/*
* Turn off this device
*/
int DeviceGroup::turnOff(void) {
	int retVal = TELLSTICK_ERROR_UNKNOWN;
	
	for (DeviceList::const_iterator it = deviceList.begin(); it != deviceList.end(); ++it) {
		int methods = (*it)->methods();
		if (methods & TELLSTICK_TURNOFF) {
			int success = (*it)->switchState( TELLSTICK_TURNOFF );
			if (retVal != TELLSTICK_SUCCESS) {
				retVal = success;
			}
		}
	}
	return retVal;
}

/*
* Send a bell
*/
int DeviceGroup::bell(void){
	int retVal = TELLSTICK_ERROR_UNKNOWN;
	
	for (DeviceList::const_iterator it = deviceList.begin(); it != deviceList.end(); ++it) {
		int methods = (*it)->methods();
		if (methods & TELLSTICK_BELL) {
			int success = (*it)->switchState( TELLSTICK_BELL );
			if (retVal != TELLSTICK_SUCCESS) {
				retVal = success;
			}
		}
	}
	return retVal;
}

/*
* Turn off this device
*/
int DeviceGroup::dim(unsigned char level){
	int retVal = TELLSTICK_ERROR_UNKNOWN;
	
	for (DeviceList::const_iterator it = deviceList.begin(); it != deviceList.end(); ++it) {
		int methods = (*it)->methods();
		if (methods & TELLSTICK_DIM) {
			int success = (*it)->switchState( TELLSTICK_DIM, (char*)&level);
			if (retVal != TELLSTICK_SUCCESS) {
				retVal = success;
			}
		}
	}
	return retVal;
}

bool DeviceGroup::parameterMatches( const std::string &name, const std::string &value ) const {
	return false;
}

/*
* Has the device got the method?
*/
int DeviceGroup::methods(){
	int retVal = 0;
	
	for (DeviceList::const_iterator it = deviceList.begin(); it != deviceList.end(); ++it) {
		retVal = retVal | (*it)->methods();
	}
	
	return retVal;
}

std::string DeviceGroup::getProtocol() const {
	return "group";
}

