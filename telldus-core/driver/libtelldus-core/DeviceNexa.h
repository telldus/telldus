#pragma once
#include "Device.h"
#include <string>

class DeviceNexa : public Device
{
public:
	DeviceNexa(int model, const std::string &strHouse, const std::string &strCode);
	virtual int turnOn(void);
	virtual int turnOff(void);
	virtual int bell(void);
	virtual int methods(int methodsSupported);
	virtual std::string getProtocol();
	
public:
	~DeviceNexa(void);

protected:
	int intHouse;
	int intCode;
	std::string getStringCode(int);
};

