#pragma once
#include "Device.h"
#include <string>


class DeviceSartano : public Device
{
public:
	DeviceSartano(int intSystem, int intCode, int intDeviceIndex);
	virtual void turnOn(void);
	virtual void turnOff(void);
	virtual bool hasMethod(int methodname, char* strModel);

	~DeviceSartano(void);

protected:
	std::string getStringCode(int);

	int intSystem;
	int intCode;
};
