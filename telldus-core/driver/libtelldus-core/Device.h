#pragma once

#include "telldus-core.h"
//#include <QMutex>

class Device
{
public:
	Device();
	~Device(void);
	
	virtual int turnOn(void);
	virtual int turnOff(void);
	virtual int bell(void);
	virtual int dim(unsigned char level);
	virtual int methods(int intModel, int methodsSupported) = 0;
	
#ifdef _LINUX
	void setDevice(const char *device);
protected:
	char *strDevice;
#endif

protected:
	int send(char* strMessage);

//private:
//	static QMutex deviceMutex;
};
