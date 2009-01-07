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

int Device::getModel() {
	return model;
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
