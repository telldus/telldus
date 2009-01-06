#pragma once
#include "Device.h"
#include <string>


class DeviceSartano : public Device
{
public:
	DeviceSartano(char *strCode);
	virtual int turnOn(void);
	virtual int turnOff(void);
	virtual int methods(int methodsSupported);
	virtual std::string getProtocol();

	~DeviceSartano(void);

protected:
	std::string getStringCode(void);
	std::string strCode;
};
