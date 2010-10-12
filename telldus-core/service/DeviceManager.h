#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

class DeviceManager
{
public:
	DeviceManager(void);
	~DeviceManager(void);

private:

	void fillDevices(void);

	class PrivateData;
	PrivateData *d;
};

#endif //DEVICEMANAGER_H