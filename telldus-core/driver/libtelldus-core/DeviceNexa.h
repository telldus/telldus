#pragma once
#include "Device.h"
#include <string>

class DeviceNexa : public Device
{
public:
	DeviceNexa(char *strHouse, char *strCode);
	virtual int turnOn(void);
	virtual int turnOff(void);
	virtual int bell(void);
	virtual int methods(int intModel);
	
public:
	~DeviceNexa(void);

protected:
	int intHouse;
	int intCode;
	std::string getStringCode(int);
};

