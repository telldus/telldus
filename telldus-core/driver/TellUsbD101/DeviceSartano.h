#pragma once
#include "Device.h"
#include <string>


class DeviceSartano : public Device
{
public:
	DeviceSartano(char *strCode);
	virtual void turnOn(void);
	virtual void turnOff(void);
	virtual int methods(char* strModel);

	~DeviceSartano(void);

protected:
	std::string getStringCode(void);
	std::string strCode;
};
