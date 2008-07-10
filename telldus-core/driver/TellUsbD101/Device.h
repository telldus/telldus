#pragma once

#include "TellUsbD101.h"

class Device
{
public:
	Device();
	~Device(void);
	
	virtual void turnOn(void);
	virtual void turnOff(void);
	virtual void bell(void);
	virtual void dim(unsigned char level);
	virtual int methods(char* strModel);
	
#ifndef _WINDOWS
	void setDevice(const char *device);
protected:
	char *strDevice;
#endif

protected:
	void send(char* strMessage);
};
