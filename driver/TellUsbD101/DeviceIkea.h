#pragma once
#include "Device.h"
#include <string>

class DeviceIkea : public Device
{
public:
	DeviceIkea(int intSystem, int intUnits, int fadeStyle, int intDeviceIndex);
	virtual void turnOn(void);
	virtual void turnOff(void);
	virtual void dim(unsigned char level);
	virtual int methods(char* strModel);
	
public:
	~DeviceIkea(void);

protected:
	int intSystem;
	int intUnits;
	int intFadeStyle;
	std::string getStringCode(unsigned char);
};

