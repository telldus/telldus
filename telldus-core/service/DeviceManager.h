#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager(void);

private:
	class PrivateData;
	PrivateData *d;
};

#endif //DEVICEMANAGER_H