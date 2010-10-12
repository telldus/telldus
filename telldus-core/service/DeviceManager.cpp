#include "DeviceManager.h"
#include "Device.h"

#include <map>

class DeviceManager::PrivateData {
public:
	std::map<int, Device> devices;
};

DeviceManager::DeviceManager(){
	d = new PrivateData;
	fillDevices();
}

DeviceManager::~DeviceManager(void) {
	//delete d->devices;
	delete d;
}

void DeviceManager::fillDevices(){
	//foreach device i registret
		//hämta id, låt devicen själv sätta sina värden i constructorn
		//lägg till i devices-listan
}
