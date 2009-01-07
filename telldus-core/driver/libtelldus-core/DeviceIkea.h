#pragma once
#include "Device.h"
#include <string>

class DeviceIkea : public Device
{
public:
	DeviceIkea(const std::string &strSystem, const std::string &strUnits, const std::string &strFadeStyle);
	virtual int turnOn(void);
	virtual int turnOff(void);
	virtual int dim(unsigned char level);
	virtual int methods(int methodsSupported);
	virtual std::string getProtocol();
	
public:
	~DeviceIkea(void);

protected:
	int intSystem;
	int intUnits;
	int intFadeStyle;
	std::string getStringCode(unsigned char);
};

