#include "DeviceManager.h"
#include "Settings.h"

#include <map>

typedef std::map<int, Device *> DeviceMap;

class DeviceManager::PrivateData {
public:
	 DeviceMap devices;
};

DeviceManager::DeviceManager(){
	d = new PrivateData;
	fillDevices();
}

DeviceManager::~DeviceManager(void) {
	for (DeviceMap::iterator it = d->devices.begin(); it != d->devices.end(); ++it) {
		delete( it->second );
	}
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
		d->devices[id]->setPreferredControllerId(set.getPreferredControllerId(id));
		d->devices[id]->setParameter(L"house", set.getDeviceParameter(id, L"house"));
		d->devices[id]->setParameter(L"unit", set.getDeviceParameter(id, L"unit"));
		d->devices[id]->setParameter(L"code", set.getDeviceParameter(id, L"code"));
		d->devices[id]->setParameter(L"units", set.getDeviceParameter(id, L"units"));
		d->devices[id]->setParameter(L"fade", set.getDeviceParameter(id, L"fade"));
		d->devices[id]->setParameter(L"system", set.getDeviceParameter(id, L"system"));
	}
}

int DeviceManager::getNumberOfDevices(){
	return (int)d->devices.size();
}

Device *DeviceManager::getDevice(int deviceId){
	
	if (!d->devices.size()) {
		return 0;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		return it->second;
	}
	return 0;	//not found
}

bool DeviceManager::addDevice(){
	Settings set;

	//TODO: Lock?
	int id = set.addDevice();
	if(id == -1){
		return false;
	}

	//TODO: Lock
	d->devices[id] = new Device(id);
	if(!d->devices[id]){
		return false;
	}
	return true;
	//release locks
}
