#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "Device.h"
#include "ControllerManager.h"

class DeviceManager
{
public:
	DeviceManager(ControllerManager *controllerManager);
	~DeviceManager(void);
	int getNumberOfDevices(void);
	int addDevice();
	int getDeviceId(int deviceIndex);
	int getDeviceType(int deviceId);
	int getPreferredControllerId(int deviceId);
	int removeDevice(int deviceId);
	int doAction(int deviceId, int action, unsigned char data);

private:

	void fillDevices(void);
	
	class PrivateData;
	PrivateData *d;
};

#endif //DEVICEMANAGER_H