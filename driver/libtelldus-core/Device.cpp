#include "Device.h"
#include <stdlib.h>

/*
* Constructor
*/
Device::Device()
{
#ifdef _LINUX
	strDevice = 0;
#endif
}

/*
* Destructor
*/
Device::~Device(void)
{
#ifdef _LINUX
	if (strDevice != 0) {
		free(strDevice);
	}
#endif
}

int Device::getModel() {
	return 1;
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
