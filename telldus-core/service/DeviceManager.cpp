#include "DeviceManager.h"
#include "Settings.h"

#include <map>

class DeviceManager::PrivateData {
public:
	std::map<int, Device *> devices;
};

DeviceManager::DeviceManager(){
	d = new PrivateData;
	fillDevices();
}

DeviceManager::~DeviceManager(void) {
	//delete d->devices;
	//todo: delete devices
	delete d;
}

void DeviceManager::fillDevices(){
	Settings set;
	int numberOfDevices = set.getNumberOfDevices();

	for (int i = 0; i < numberOfDevices; ++i) {
		int id = set.getDeviceId(i);
		d->devices[id] = new Device(id);
		d->devices[id]->setName(set.getName(id));
		d->devices[id]->setModel(set.getModel(id));
		d->devices[id]->setProtocolName(set.getProtocol(id));
		d->devices[id]->setParameter(L"house", set.getDeviceParameter(id, L"house"));
		d->devices[id]->setParameter(L"unit", set.getDeviceParameter(id, L"unit"));
		d->devices[id]->setParameter(L"code", set.getDeviceParameter(id, L"code"));
		d->devices[id]->setParameter(L"units", set.getDeviceParameter(id, L"units"));
		d->devices[id]->setParameter(L"fade", set.getDeviceParameter(id, L"fade"));
		d->devices[id]->setParameter(L"system", set.getDeviceParameter(id, L"system"));
	}
}

int DeviceManager::getNumberOfDevices(){
	return d->devices.size();
}

Device *DeviceManager::getDevice(int deviceId){
	return d->devices[deviceId];
}
