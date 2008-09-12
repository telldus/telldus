#include "Device.h"

/*
* Constructor
*/
Device::Device()
{
}

/*
* Destructor
*/
Device::~Device(void)
{
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
