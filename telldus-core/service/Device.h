#ifndef DEVICE_H
#define DEVICE_H

class Device
{
public:
	Device();
	~Device(void);

private:
	class PrivateData;
	PrivateData *d;
};

#endif //DEVICE_H