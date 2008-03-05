#pragma once
#include "DeviceNexa.h"
#include <string>

class DeviceWaveman : public DeviceNexa
{
public:
	DeviceWaveman(char *strHouse, char *strCode);
	virtual void turnOff(void);
	virtual int methods(char* strModel);
};
