#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "Device.h"

class DeviceManager
{
public:
	DeviceManager(void);
	~DeviceManager(void);
	int getNumberOfDevices(void);
	Device *getDevice(int deviceId);

private:

	void fillDevices(void);
	
	class PrivateData;
	PrivateData *d;
};

#endif //DEVICEMANAGER_H