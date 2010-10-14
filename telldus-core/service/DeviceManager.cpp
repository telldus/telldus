#include "DeviceManager.h"
#include "Mutex.h"
#include "Settings.h"

#include <map>

typedef std::map<int, Device *> DeviceMap;

class DeviceManager::PrivateData {
public:
	 DeviceMap devices;
	 Settings set;		//TODO: have to be here, right? Only one instance?
};

DeviceManager::DeviceManager(){
	d = new PrivateData;
	fillDevices();
}

DeviceManager::~DeviceManager(void) {
	//TODO: Locks
	//always lock list first, device after (or only one of them)
	for (DeviceMap::iterator it = d->devices.begin(); it != d->devices.end(); ++it) {
		delete( it->second );
	}
	delete d;
}

void DeviceManager::fillDevices(){
	int numberOfDevices = d->set.getNumberOfDevices();

	for (int i = 0; i < numberOfDevices; ++i) {
		int id = d->set.getDeviceId(i);
		d->devices[id] = new Device(id);
		d->devices[id]->setName(d->set.getName(id));
		d->devices[id]->setModel(d->set.getModel(id));
		d->devices[id]->setProtocolName(d->set.getProtocol(id));
		d->devices[id]->setPreferredControllerId(d->set.getPreferredControllerId(id));
		d->devices[id]->setParameter(L"house", d->set.getDeviceParameter(id, L"house"));
		d->devices[id]->setParameter(L"unit", d->set.getDeviceParameter(id, L"unit"));
		d->devices[id]->setParameter(L"code", d->set.getDeviceParameter(id, L"code"));
		d->devices[id]->setParameter(L"units", d->set.getDeviceParameter(id, L"units"));
		d->devices[id]->setParameter(L"fade", d->set.getDeviceParameter(id, L"fade"));
		d->devices[id]->setParameter(L"system", d->set.getDeviceParameter(id, L"system"));
	}
}

int DeviceManager::getNumberOfDevices(){
	return (int)d->devices.size();
}

bool DeviceManager::addDevice(){
	Settings set;

	//TODO: Lock? (set too)
	int id = d->set.addDevice();
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

int DeviceManager::getDeviceId(int intDeviceIndex) {
	//TODO This must be done differently, device index (why is it even needed?) may be stored in device too	
	return d->set.getDeviceId(intDeviceIndex);
}

int DeviceManager::getPreferredControllerId(int deviceId){
	
	//TODO: repeating code...
	//TODO: Lock
	if (!d->devices.size()) {
			return 0;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		return it->second->getPreferredControllerId();
	}
	return 0;	//not found

}

bool DeviceManager::removeDevice(int deviceId){
	
	//TODO: Lock device/list and set?
	if(d->set.removeDevice(deviceId)){
		//TODO ta bort från listan d->devices[id]
	}
	

	return false;
}

bool DeviceManager::turnOn(int deviceId, Controller *controller){
	
	
	{
		//TODO: Get device
		//TelldusCore::MutexLocker locker(&device);
		//device is locked in current scope
		
		if (!d->devices.size()) {
				return false;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {
			return it->second->turnOn(controller);
		}
		return false;	//not found
		
	}
	
}
