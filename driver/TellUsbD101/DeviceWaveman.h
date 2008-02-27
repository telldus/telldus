#pragma once
#include "DeviceNexa.h"
#include <string>

class DeviceWaveman : public DeviceNexa
{
public:
	DeviceWaveman(int intHouse, int intCode, int intDeviceIndex);
	virtual void turnOff(void);
	virtual int methods(char* strModel);
};
