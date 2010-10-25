#include "DeviceManager.h"
#include "ControllerMessage.h"
#include "Mutex.h"
#include "Settings.h"
#include "Strings.h"
#include "Message.h"
#include "common.h"

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
	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	for (DeviceMap::iterator it = d->devices.begin(); it != d->devices.end(); ++it) {
		{TelldusCore::MutexLocker deviceLocker(it->second);}	//aquire lock, and release it, just to see that the device it's not in use anywhere
		delete(it->second);
	}
	delete d;
}

void DeviceManager::fillDevices(){
	int numberOfDevices = d->set.getNumberOfDevices();
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	
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
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		return it->second->getLastSentCommand(methodsSupported);
	}
	return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
}

int DeviceManager::setDeviceLastSentCommand(int deviceId, int command, std::wstring value)
{	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		d->set.setDeviceState(deviceId, command,value);
		it->second->setLastSentCommand(command, value);
	}
	else{
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	return TELLSTICK_SUCCESS;
}

std::wstring DeviceManager::getDeviceStateValue(int deviceId){
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return L"UNKNOWN";
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		return it->second->getStateValue();
	}
	return L"UNKNOWN";
}

int DeviceManager::getDeviceMethods(int deviceId, int methodsSupported){
	//devices locked
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	
	if (!d->devices.size()) {
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		int methods = it->second->getMethods();
		return Device::maskUnsupportedMethods(methods, methodsSupported);
	}

	return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
}

std::wstring DeviceManager::getDeviceModel(int deviceId){
	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return L"UNKNOWN";
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		return it->second->getModel();
	}
	return L"UNKNOWN";
}

int DeviceManager::setDeviceModel(int deviceId, std::wstring model)
{	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		d->set.setModel(deviceId, model);
		it->second->setModel(model);
	}
	else{
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}

	return TELLSTICK_SUCCESS;
}

std::wstring DeviceManager::getDeviceName(int deviceId){
	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return L"UNKNOWN";
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		return it->second->getName();
	}
	return L"UNKNOWN";
}

int DeviceManager::setDeviceName(int deviceId, std::wstring name){
	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		d->set.setName(deviceId, name);
		it->second->setName(name);
	}
	else{
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	
	return TELLSTICK_SUCCESS;
}

std::wstring DeviceManager::getDeviceParameter(int deviceId, std::wstring name, std::wstring defaultValue){
	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return defaultValue;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()){
		TelldusCore::MutexLocker deviceLocker(it->second);
		std::wstring returnString = it->second->getParameter(name);
		if(returnString != L""){
			return returnString;
		}
	}
	return defaultValue;
}

int DeviceManager::setDeviceParameter(int deviceId, std::wstring name, std::wstring value)
{	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		d->set.setDeviceParameter(deviceId, name, value);
		it->second->setParameter(name, value);
	}
	else{
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}

	return TELLSTICK_SUCCESS;
}

std::wstring DeviceManager::getDeviceProtocol(int deviceId){
	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return L"UNKNOWN";
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		return it->second->getProtocolName();
	}
	return L"UNKNOWN";
}

int DeviceManager::setDeviceProtocol(int deviceId, std::wstring protocol)
{	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		d->set.setProtocol(deviceId, protocol);
		it->second->setProtocolName(protocol);
	}
	else{
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}

	return TELLSTICK_SUCCESS;
}

int DeviceManager::getNumberOfDevices(){
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	return (int)d->devices.size();
}

int DeviceManager::addDevice(){
	
	int id = d->set.addDevice();
	if(id == -1){
		return TELLSTICK_ERROR_UNKNOWN;
	}

	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	d->devices[id] = new Device(id);
	if(!d->devices[id]){
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	return id;
}

int DeviceManager::getDeviceId(int deviceIndex) {
	return d->set.getDeviceId(deviceIndex);
}

int DeviceManager::getDeviceType(int deviceId){

	return TELLSTICK_TYPE_DEVICE;
}

int DeviceManager::getPreferredControllerId(int deviceId){
	
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
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
	//On the stack and will be released if we have a device lock.
	std::auto_ptr<TelldusCore::MutexLocker> deviceLocker(0);
	{ 
		//devicelist locked
		TelldusCore::MutexLocker deviceListLocker(&d->lock);
		
		if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it == d->devices.end()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;	//not found
		}
		//device locked
		deviceLocker = std::auto_ptr<TelldusCore::MutexLocker>(new TelldusCore::MutexLocker(it->second));
		device = it->second;
	} //devicelist unlocked
	
	Controller *controller = d->controllerManager->getBestControllerById(this->getPreferredControllerId(deviceId));
	if(controller){
		int retval = device->doAction(action, data, controller);
		if(retval == TELLSTICK_SUCCESS) {
			std::wstring datastring = TelldusCore::Message::charUnsignedToWstring(data);
			if (this->triggerDeviceStateChange(deviceId, action, datastring)) {
				device->setLastSentCommand(action, datastring);
				d->set.setDeviceState(deviceId, action, datastring);
			}
		}
		return retval;
	} else {
		return TELLSTICK_ERROR_NOT_FOUND;
	}
}

int DeviceManager::removeDevice(int deviceId){
	
	Device *device = 0;
	{
		if(!d->set.removeDevice(deviceId)){		//remove from register/settings
			return TELLSTICK_ERROR_UNKNOWN;
		}

		TelldusCore::MutexLocker deviceListLocker(&d->lock);
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

	return TELLSTICK_DEVICE_REMOVED;
}

void DeviceManager::handleControllerMessage(const ControllerEventData &eventData) {
	//Trigger raw-event
	EventUpdateData *eventUpdateData = new EventUpdateData();
	eventUpdateData->messageType = L"TDRawDeviceEvent";
	eventUpdateData->controllerId = 0; //TODO add the real controller-id
	eventUpdateData->eventValue = TelldusCore::charToWstring(eventData.msg.c_str());
	d->deviceUpdateEvent->signal(eventUpdateData);

	ControllerMessage msg(eventData.msg);
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	for (DeviceMap::iterator it = d->devices.begin(); it != d->devices.end(); ++it) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		if (!TelldusCore::comparei(it->second->getProtocolName(), msg.protocol())) {
			continue;
		}
		if (! (it->second->getMethods() & msg.method())) {
			continue;
		}

		std::list<std::string> parameters = it->second->getParametersForProtocol();
		bool thisDevice = true;
		for (std::list<std::string>::iterator paramIt = parameters.begin(); paramIt != parameters.end(); ++paramIt){
			//TODO more efficient conversion/compare? wstring or not?
			if(!TelldusCore::comparei(it->second->getParameter(TelldusCore::charToWstring((*paramIt).c_str())), TelldusCore::charToWstring(msg.getParameter(*paramIt).c_str()))){
				thisDevice = false;
				break;
			}
		}

		if(!thisDevice){
			continue;
		}

		if (this->triggerDeviceStateChange(it->first, msg.method(), L"")) {
			d->set.setDeviceState(it->first, msg.method(), L"");
			setDeviceLastSentCommand(it->first, msg.method(), L"");	//TODO value, when implemented
		}
		break;
	}
}

int DeviceManager::sendRawCommand(std::wstring command, int reserved){
	
	//TODO test this!
	Controller *controller = d->controllerManager->getBestControllerById(-1);
	if(controller){
		return controller->send(TelldusCore::wideToString(command));	// std::string(command.begin(), command.end()));
	}
	else{
		return TELLSTICK_ERROR_NOT_FOUND;
	}
}

bool DeviceManager::triggerDeviceStateChange(int deviceId, int intDeviceState, const std::wstring &strDeviceStateValue ) {
	if ( intDeviceState == TELLSTICK_BELL || intDeviceState == TELLSTICK_LEARN) {
		return false;
	}

	EventUpdateData *eventData = new EventUpdateData();
	eventData->messageType = L"TDDeviceEvent";
	eventData->eventState = intDeviceState;
	eventData->deviceId = deviceId;
	eventData->eventValue = strDeviceStateValue;
	d->deviceUpdateEvent->signal(eventData);
	return true;
}
