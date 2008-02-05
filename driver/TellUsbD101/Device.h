#pragma once

#include "TellUsbD101.h"

class Device
{
private:
	int intDongleIndex;
protected:
	void send(char* strMessage);
public:
	Device(int intDeviceIndex);
	virtual void turnOn(void);
	virtual void turnOff(void);
	virtual void bell(void);
	virtual void dim(unsigned char level);
	virtual bool hasMethod(int methodname, char* strModel);
	static int getDongleIndex();

	static void debugLog(char* debugstring);

public:
	~Device(void);
};
