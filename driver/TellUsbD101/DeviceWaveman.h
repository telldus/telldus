#pragma once
#include "DeviceNexa.h"
#include <string>

class DeviceWaveman : public DeviceNexa
{
public:
	DeviceWaveman(int intHouse, int intCode, int intDeviceIndex);
	virtual void turnOff(void);
	virtual bool hasMethod(int methodname, char* strModel);
};
