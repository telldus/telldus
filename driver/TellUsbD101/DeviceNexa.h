#pragma once
#include "Device.h"
#include <string>

class DeviceNexa : public Device
{
public:
	DeviceNexa(int intHouse, int intCode, int intDeviceIndex);
	virtual void turnOn(void);
	virtual void turnOff(void);
	virtual void bell(void);
	virtual bool hasMethod(int methodname, char* strModel);
	
public:
	~DeviceNexa(void);

protected:
	int intHouse;
	int intCode;
	std::string getStringCode(int);
};

