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

/*
* Turn on, virtual
*/
int Device::turnOn(void){
	//do nothing
}

/*
* Turn off, virtual
*/
int Device::turnOff(void){
	//do nothing
}

/*
* Bell, virtual
*/
int Device::bell(void){
	//do nothing
}

/*
* Dim, virtual
*/
int Device::dim(unsigned char level){
	//do nothing
}

/*
* Methods, virtual
*/
int Device::methods(char* strModel){
	return 0;
}
