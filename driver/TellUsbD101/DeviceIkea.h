#pragma once
#include "Device.h"
#include <string>

class DeviceIkea : public Device
{
public:
	DeviceIkea(char *strSystem, char *strUnits, char *strFadeStyle);
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

