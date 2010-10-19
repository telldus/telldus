#include "DeviceManager.h"
#include "Mutex.h"
#include "Settings.h"
#include "Message.h"

#include <map>

typedef std::map<int, Device *> DeviceMap;

class DeviceManager::PrivateData {
public:
	 DeviceMap devices;
	 Settings set;
	 TelldusCore::Mutex lock;
	 ControllerManager *controllerManager;
	 Event *deviceUpdateEvent;
};

DeviceManager::DeviceManager(ControllerManager *controllerManager, Event *deviceUpdateEvent){
	d = new PrivateData;
	d->controllerManager = controllerManager;
	d->deviceUpdateEvent = deviceUpdateEvent;
	fillDevices();
}

DeviceManager::~DeviceManager(void) {
	
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	for (DeviceMap::iterator it = d->devices.begin(); it != d->devices.end(); ++it) {
		{TelldusCore::MutexLocker lock(it->second);}	//aquire lock, and release it, just to see that the device it's not in use anywhere
		delete(it->second);
	}
	delete d;
}

void DeviceManager::fillDevices(){
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	int numberOfDevices = d->set.getNumberOfDevices();

	for (int i = 0; i < numberOfDevices; ++i) {
		int id = d->set.getDeviceId(i);
		d->devices[id] = new Device(id);
		d->devices[id]->setName(d->set.getName(id));
		d->devices[id]->setModel(d->set.getModel(id));
		d->devices[id]->setProtocolName(d->set.getProtocol(id));
		d->devices[id]->setPreferredControllerId(d->set.getPreferredControllerId(id));
		d->devices[id]->setLastSentCommand(d->set.getDeviceState(id), d->set.getDeviceStateValue(id));
		d->devices[id]->setParameter(L"house", d->set.getDeviceParameter(id, L"house"));
		d->devices[id]->setParameter(L"unit", d->set.getDeviceParameter(id, L"unit"));
		d->devices[id]->setParameter(L"code", d->set.getDeviceParameter(id, L"code"));
		d->devices[id]->setParameter(L"units", d->set.getDeviceParameter(id, L"units"));
		d->devices[id]->setParameter(L"fade", d->set.getDeviceParameter(id, L"fade"));
		d->devices[id]->setParameter(L"system", d->set.getDeviceParameter(id, L"system"));
	}
}

int DeviceManager::getDeviceLastSentCommand(int deviceId, int methodsSupported){
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		//TODO: Is it ok NOT to get a lock here? Should be, since the list is locked, and it's an fast operation?
		return it->second->getLastSentCommand(methodsSupported);
	}
	return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
}

int DeviceManager::setDeviceLastSentCommand(int deviceId, int command, std::wstring value)
{	
	//TODO: check this locking, ok?
	Device *device = 0;
	{
		TelldusCore::MutexLocker deviceLocker(&d->lock);
		if (!d->devices.size()) {
				return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {
			device = it->second;
			d->set.setDeviceState(deviceId, command,value);
		}
		else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	if(device){
		TelldusCore::MutexLocker lock(device);
		device->setLastSentCommand(command, value);
	}
	
	return TELLSTICK_SUCCESS;
}



std::wstring DeviceManager::getDeviceStateValue(int deviceId){
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	if (!d->devices.size()) {
			return L"UNKNOWN";	//TODO, what?
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		//TODO: Is it ok NOT to get a lock here? Should be, since the list is locked, and it's an fast operation?
		return it->second->getStateValue();
	}
	return L"UNKNOWN";	//TODO, what?
}

int DeviceManager::getDeviceMethods(int deviceId, int methodsSupported){
	Device *device = 0;
	{ 
		//devices locked
		TelldusCore::MutexLocker devicesLocker(&d->lock);
		
		if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {
			it->second->lock();	//device locked TODO
			device = it->second;
		}
		//devices unlocked
	}

	if (!device) {
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;	//not found
	}
	int methods = device->getMethods();
	device->unlock();
	return Device::maskUnsupportedMethods(methods, methodsSupported);

}

std::wstring DeviceManager::getDeviceModel(int deviceId){
	
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	if (!d->devices.size()) {
			return L"UNKNOWN";	//TODO, what?
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		//TODO: Is it ok NOT to get a lock here? Should be, since the list is locked, and it's an fast operation?
		return it->second->getModel();
	}
	return L"UNKNOWN";	//TODO, what?
}

int DeviceManager::setDeviceModel(int deviceId, std::wstring model)
{	
	//TODO: check this locking, ok?
	Device *device = 0;
	{
		TelldusCore::MutexLocker deviceLocker(&d->lock);
		if (!d->devices.size()) {
				return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {
			device = it->second;
			d->set.setModel(deviceId, model);
		}
		else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	if(device){
		TelldusCore::MutexLocker lock(device);
		device->setModel(model);
		//TODO Reset protocol here?
	}
	
	return TELLSTICK_SUCCESS;
}

std::wstring DeviceManager::getDeviceName(int deviceId){
	
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	if (!d->devices.size()) {
			return L"UNKNOWN";	//TODO, what?
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		//TODO: Is it ok NOT to get a lock here? Should be, since the list is locked, and it's an fast operation?
		return it->second->getName();
	}
	return L"UNKNOWN";	//TODO, what?
}

int DeviceManager::setDeviceName(int deviceId, std::wstring name){
	
	//TODO: check this locking, ok?
	Device *device = 0;
	{
		TelldusCore::MutexLocker deviceLocker(&d->lock);
		if (!d->devices.size()) {
				return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {
			device = it->second;
			d->set.setName(deviceId, name);
		}
		else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	if(device){
		TelldusCore::MutexLocker lock(device);
		device->setName(name);
	}
	
	return TELLSTICK_SUCCESS;
}

std::wstring DeviceManager::getDeviceParameter(int deviceId, std::wstring name, std::wstring defaultValue){
	
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	if (!d->devices.size()) {
			return defaultValue;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()){
		//TODO: Is it ok NOT to get a lock here? Should be, since the list is locked, and it's an fast operation?
		std::wstring returnString = it->second->getParameter(name);
		if(returnString != L""){
			return returnString;
		}
	}
	return defaultValue;
}

int DeviceManager::setDeviceParameter(int deviceId, std::wstring name, std::wstring value)
{	
	//TODO: check this locking, ok?
	Device *device = 0;
	{
		TelldusCore::MutexLocker deviceLocker(&d->lock);
		if (!d->devices.size()) {
				return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {
			device = it->second;
			d->set.setDeviceParameter(deviceId, name, value);
		}
		else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	if(device){
		TelldusCore::MutexLocker lock(device);
		device->setParameter(name, value);
		//TODO Reset protocol here?
	}
	
	return TELLSTICK_SUCCESS;
}

std::wstring DeviceManager::getDeviceProtocol(int deviceId){
	
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	if (!d->devices.size()) {
			return L"UNKNOWN";	//TODO, what?
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		//TODO: Is it ok NOT to get a lock here? Should be, since the list is locked, and it's an fast operation?
		return it->second->getProtocolName();
	}
	return L"UNKNOWN";	//TODO, what?
}

int DeviceManager::setDeviceProtocol(int deviceId, std::wstring protocol)
{	
	//TODO: check this locking, ok?
	Device *device = 0;
	{
		TelldusCore::MutexLocker deviceLocker(&d->lock);
		if (!d->devices.size()) {
				return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {
			device = it->second;
			d->set.setProtocol(deviceId, protocol);
		}
		else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	if(device){
		TelldusCore::MutexLocker lock(device);
		device->setProtocolName(protocol);
	}
	
	return TELLSTICK_SUCCESS;
}

int DeviceManager::getNumberOfDevices(){
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	return (int)d->devices.size();
}

int DeviceManager::addDevice(){
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	
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
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	return d->set.getDeviceId(deviceIndex);
}

int DeviceManager::getDeviceType(int deviceId){

	return TELLSTICK_TYPE_DEVICE;
}

int DeviceManager::getPreferredControllerId(int deviceId){
	
	TelldusCore::MutexLocker deviceLocker(&d->lock);
	
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		//TODO: Is it ok NOT to get a lock here? Should be, since the list is locked, and it's an fast operation?
		return it->second->getPreferredControllerId();
	}
	return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
}

void DeviceManager::connectTellStickController(int vid, int pid, std::wstring serial){
	d->controllerManager->deviceInsertedOrRemoved(vid, pid, true);
}

void DeviceManager::disconnectTellStickController(int vid, int pid, std::wstring serial){
	d->controllerManager->deviceInsertedOrRemoved(vid, pid, false);
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
	
	Controller *controller = d->controllerManager->getBestControllerById(this->getPreferredControllerId(deviceId));
	if(controller){
		int retval = device->doAction(action, data, controller);
		if(retval == TELLSTICK_SUCCESS){
			std::wstring datastring = TelldusCore::Message::charUnsignedToWstring(data);
			device->setLastSentCommand(action, datastring);
			device->unlock();
			{
				//new lock, for setting (all other locks are unlocked)
				//TODO, it MAY be inconsistency between stored values and device values here... change something?
				TelldusCore::MutexLocker deviceLocker(&d->lock);
				d->set.setDeviceState(deviceId, action, datastring);
			}
			
			EventUpdateData *eventData = new EventUpdateData();
			eventData->messageType = L"TDDeviceEvent";
			eventData->eventState = action;
			eventData->deviceId = deviceId;
			eventData->eventValue = datastring;
			d->deviceUpdateEvent->signal(eventData);
		}
		device->unlock();
		return retval;
	}
	else{
		device->unlock();
		return TELLSTICK_ERROR_NOT_FOUND;
	}
}

int DeviceManager::removeDevice(int deviceId){
	
	Device *device = 0;
	{
		TelldusCore::MutexLocker deviceLocker(&d->lock);
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

int DeviceManager::sendRawCommand(std::wstring command, int reserved){
	
	//TODO test this!
	Controller *controller = d->controllerManager->getBestControllerById(-1);
	if(controller){
		return controller->send(TelldusCore::Message::wideToString(command));	// std::string(command.begin(), command.end()));
	}
	else{
		return TELLSTICK_ERROR_NOT_FOUND;
	}
}

