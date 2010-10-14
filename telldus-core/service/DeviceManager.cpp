#include "DeviceManager.h"
#include "Mutex.h"
#include "Settings.h"

#include <map>

typedef std::map<int, Device *> DeviceMap;

class DeviceManager::PrivateData {
public:
	 DeviceMap devices;
	 Settings set;
	 TelldusCore::Mutex lock;
	 ControllerManager *controllerManager;
};

DeviceManager::DeviceManager(ControllerManager *controllerManager){
	d = new PrivateData;
	d->controllerManager = controllerManager;
	fillDevices();
}

DeviceManager::~DeviceManager(void) {
	
	TelldusCore::MutexLocker locker(&d->lock);
	for (DeviceMap::iterator it = d->devices.begin(); it != d->devices.end(); ++it) {
		{TelldusCore::MutexLocker lock(it->second);}	//aquire lock, and release it, just to see that the device it's not in use anywhere
		delete(it->second);
	}
	delete d;
}

void DeviceManager::fillDevices(){
	TelldusCore::MutexLocker locker(&d->lock);
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
	TelldusCore::MutexLocker locker(&d->lock);
	return (int)d->devices.size();
}

int DeviceManager::addDevice(){
	TelldusCore::MutexLocker locker(&d->lock);
	
	int id = d->set.addDevice();
	if(id == -1){
		return TELLSTICK_ERROR_UNKNOWN;
	}

	d->devices[id] = new Device(id);
	if(!d->devices[id]){
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	return TELLSTICK_DEVICE_ADDED;
}

int DeviceManager::getDeviceId(int deviceIndex) {
	TelldusCore::MutexLocker locker(&d->lock);
	return d->set.getDeviceId(deviceIndex);
}

int DeviceManager::getDeviceType(int deviceId){

	return TELLSTICK_TYPE_DEVICE;
}

int DeviceManager::getPreferredControllerId(int deviceId){
	
	TelldusCore::MutexLocker locker(&d->lock);
	
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		//TODO: Is it ok NOT to get a lock here? Should be, since the list is locked, and it's an fast operation?
		return it->second->getPreferredControllerId();
	}
	return TELLSTICK_ERROR_DEVICE_NOT_FOUND;	//not found
}

int DeviceManager::removeDevice(int deviceId){
	
	Device *device = 0;
	{
		TelldusCore::MutexLocker locker(&d->lock);
		if(!d->set.removeDevice(deviceId)){		//remove from register/settings
			return TELLSTICK_ERROR_UNKNOWN;
		}

		if (!d->devices.size()) {
				return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {
			device = it->second;
			d->devices.erase(it);	//remove from list, keep reference
		}
		else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	{TelldusCore::MutexLocker lock(device);}	//waiting for device lock, if it's aquired, just unlock again. Device is removed from list, and cannot be accessed from anywhere else
	delete device;

	//1 lås lista
	//2 ta bort från registret
	//4 plocka ut device ur lista
	//* Lås upp lista
	//3 vänta på device lås
	//{TelldusCore::MutexLocker(&device);}
	//6 delete device

	return TELLSTICK_DEVICE_REMOVED;
}

int DeviceManager::doAction(int deviceId, int action, unsigned char data){
	Device *device = 0;
	{ 
		//devices locked
		TelldusCore::MutexLocker deviceLocker(&d->lock);
		
		if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {
			it->second->lock();	//device locked
			device = it->second;
		}
		//devices unlocked
	}

	if (!device) {
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;	//not found
	}
	
	//TODO: Get controller here (controllermanager -> locks its own list, controller -> lock for send etc)
	Controller *controller = d->controllerManager->getBestControllerById(this->getPreferredControllerId(deviceId));
	if(controller){
		int retval = device->doAction(action, data, controller);
		device->unlock();
		return retval;
	}
	else{
		device->unlock();
		return TELLSTICK_ERROR_NOT_FOUND;
	}
}
