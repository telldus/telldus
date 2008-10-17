#pragma once
#include "Device.h"
#include <string>

class DeviceIkea : public Device
{
public:
	DeviceIkea(char *strSystem, char *strUnits, char *strFadeStyle);
	virtual int turnOn(void);
	virtual int turnOff(void);
	virtual int dim(unsigned char level);
	virtual int methods(int intModel);
	
public:
	~DeviceIkea(void);

protected:
	int intSystem;
	int intUnits;
	int intFadeStyle;
	std::string getStringCode(unsigned char);
};

