#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "Device.h"

class DeviceManager
{
public:
	DeviceManager(void);
	~DeviceManager(void);
	int getNumberOfDevices(void);
	bool addDevice();
	int getDeviceId(int deviceIndex);
	int getPreferredControllerId(int deviceId);
	bool removeDevice(int deviceId);
	bool turnOn(int deviceId, Controller *controller);

private:

	void fillDevices(void);
	
	class PrivateData;
	PrivateData *d;
};

#endif //DEVICEMANAGER_H