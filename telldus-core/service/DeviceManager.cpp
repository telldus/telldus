#include "DeviceManager.h"
#include "Device.h"

#include <map>

class DeviceManager::PrivateData {
public:
	std::map<int, Device> devices;
};

DeviceManager::DeviceManager(){

}

DeviceManager::~DeviceManager(void) {
	//delete d->devices;
	delete d;
}
