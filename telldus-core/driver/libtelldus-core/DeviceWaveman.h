#pragma once
#include "DeviceNexa.h"
#include <string>

class DeviceWaveman : public DeviceNexa
{
public:
	DeviceWaveman(char *strHouse, char *strCode);
	virtual int turnOff(void);
	virtual int methods(int intModel);
};
