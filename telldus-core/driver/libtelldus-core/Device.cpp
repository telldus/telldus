#include "Device.h"
#include "Manager.h"
#include <stdlib.h>

using namespace TelldusCore;
/*
* Constructor
*/
Device::Device(int id, const std::string m, const std::string &name)
	: deviceId(id),
		model(m),
		deviceName(name)
{
}

/*
* Destructor
*/
Device::~Device(void) {
}

int Device::switchState( int newState, const std::string &value ) {
	int retVal = TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
	switch (newState) {
		case TELLSTICK_TURNON:
			retVal = turnOn();
			break;
		case TELLSTICK_TURNOFF:
			retVal = turnOff();
			break;
		case TELLSTICK_BELL:
			retVal = bell();
			break;
		case TELLSTICK_DIM:
			retVal = dim( value[0] );
			break;
	}
	if (retVal == TELLSTICK_SUCCESS) {
		Manager *manager = Manager::getInstance();
		manager->setDeviceState(deviceId, newState, "");
	}
	return retVal;
}

std::string Device::getModel() const {
	return model;
}

bool Device::setModel( const std::string &strModel ) {
	Manager *manager = Manager::getInstance();
	if (manager->setDeviceModel( deviceId, strModel )) {
		model = strModel;
		return true;
	}
	return false;
}

bool Device::setParameter(const std::string &strName, const std::string &strValue) {
	if (setDeviceParameter( strName, strValue )) {
		Manager *manager = Manager::getInstance();
		return manager->setDeviceParameter(deviceId, strName, strValue);
	}
	return false;
}

/*
* Turn on, virtual
*/
int Device::turnOn(void){
	return TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
}

/*
* Turn off, virtual
*/
int Device::turnOff(void){
	return TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
}

/*
* Bell, virtual
*/
int Device::bell(void){
	return TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
}

/*
* Dim, virtual
*/
int Device::dim(unsigned char level){
	return TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
}

int Device::methodId( const std::string &methodName ) {
	if (methodName.compare("turnon") == 0) {
		return TELLSTICK_TURNON;
	}
	if (methodName.compare("turnoff") == 0) {
		return TELLSTICK_TURNOFF;
	}
	if (methodName.compare("bell") == 0) {
		return TELLSTICK_BELL;
	}
	if (methodName.compare("dim") == 0) {
		return TELLSTICK_DIM;
	}
	return 0;
}

std::string Device::getName() const {
	return deviceName;
}

bool Device::setName(const std::string & newName) {
	Manager *manager = Manager::getInstance();
	if (manager->setDeviceName(deviceId, newName)) {
		deviceName = newName;
		return true;
	}
	return false;
}
