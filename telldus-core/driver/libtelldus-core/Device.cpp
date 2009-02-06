#include "Device.h"
#include <stdlib.h>

using namespace TelldusCore;
/*
* Constructor
*/
Device::Device(int m)
	: model(m)
{
}

/*
* Destructor
*/
Device::~Device(void) {
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
