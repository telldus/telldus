#ifndef DEVICE_H
#define DEVICE_H

#include "Controller.h"
#include "Protocol.h"
#include <string>

class Device
{
public:
	Device();
	~Device(void);

	std::wstring getName();

	int turnOn(Controller *controller);

private:

	Protocol *retrieveProtocol();

	class PrivateData;
	PrivateData *d;
};

#endif //DEVICE_H