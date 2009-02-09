#include "Device.h"
#include "Manager.h"
#include <stdlib.h>

using namespace TelldusCore;
/*
* Constructor
*/
Device::Device(int id, int m)
	: deviceId(id),
      model(m)
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

int Device::getModel() const {
	return model;
}

void Device::setModel( int intModel ) {
	model = intModel;
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
