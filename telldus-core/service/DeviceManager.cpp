#include "DeviceManager.h"
#include "ControllerMessage.h"
#include "Mutex.h"
#include "Sensor.h"
#include "Settings.h"
#include "Strings.h"
#include "Message.h"
#include "Log.h"

#include <map>
#include <memory>
#include <sstream>
#include <time.h>

typedef std::map<int, Device *> DeviceMap;

class DeviceManager::PrivateData {
public:
	 DeviceMap devices;
	 std::list<Sensor *> sensorList;
	 Settings set;
	 TelldusCore::Mutex lock;
	 ControllerManager *controllerManager;
	 EventRef deviceUpdateEvent;
};

DeviceManager::DeviceManager(ControllerManager *controllerManager, EventRef deviceUpdateEvent){
	d = new PrivateData;
	d->controllerManager = controllerManager;
	d->deviceUpdateEvent = deviceUpdateEvent;
	fillDevices();
}

DeviceManager::~DeviceManager(void) {
	{
		TelldusCore::MutexLocker deviceListLocker(&d->lock);
		for (DeviceMap::iterator it = d->devices.begin(); it != d->devices.end(); ++it) {
			{TelldusCore::MutexLocker deviceLocker(it->second);}	//aquire lock, and release it, just to see that the device it's not in use anywhere
			delete(it->second);
		}
		for (std::list<Sensor *>::iterator it = d->sensorList.begin(); it != d->sensorList.end(); ++it) {
			{TelldusCore::MutexLocker sensorLocker(*it);}	//aquire lock, and release it, just to see that the device it's not in use anywhere
			delete(*it);
		}
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
		d->devices[id]->setParameter(L"devices", d->set.getDeviceParameter(id, L"devices"));
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

int DeviceManager::setDeviceLastSentCommand(int deviceId, int command, const std::wstring &value)
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

int DeviceManager::getDeviceMethods(int deviceId, int methodsSupported) {
	return Device::maskUnsupportedMethods(DeviceManager::getDeviceMethods(deviceId), methodsSupported);
}

int DeviceManager::getDeviceMethods(int deviceId) {
	std::set<int> duplicateDeviceIds;
	return DeviceManager::getDeviceMethods(deviceId, duplicateDeviceIds);
}

int DeviceManager::getDeviceMethods(int deviceId, std::set<int> &duplicateDeviceIds){
	int type = 0;
	int methods = 0;
	std::wstring deviceIds;
	std::wstring protocol;

	{
		//devices locked
		TelldusCore::MutexLocker deviceListLocker(&d->lock);

		if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
		DeviceMap::iterator it = d->devices.find(deviceId);
		if (it != d->devices.end()) {

			{
				TelldusCore::MutexLocker deviceLocker(it->second);
				type = it->second->getType();
				methods = it->second->getMethods();
				deviceIds = it->second->getParameter(L"devices");
				protocol = it->second->getProtocolName();
			}
		}
	}
	if(type == 0){
		return 0;
	}
	if(type == TELLSTICK_TYPE_GROUP){

		//get all methods that some device in the groups supports
		std::wstring deviceIdBuffer;
		std::wstringstream devicesstream(deviceIds);
		methods = 0;

		duplicateDeviceIds.insert(deviceId);

		while(std::getline(devicesstream, deviceIdBuffer, L',')){
			int deviceIdInGroup = TelldusCore::wideToInteger(deviceIdBuffer);
			if(duplicateDeviceIds.count(deviceIdInGroup) == 1){
				//action for device already executed, or will execute, do nothing to avoid infinite loop
				continue;
			}

			duplicateDeviceIds.insert(deviceIdInGroup);

			int deviceMethods = getDeviceMethods(deviceIdInGroup, duplicateDeviceIds);
			if(deviceMethods > 0){
				methods |= deviceMethods;
			}
		}
	}
	return methods;
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

int DeviceManager::setDeviceModel(int deviceId, const std::wstring &model)
{
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		int ret = d->set.setModel(deviceId, model);
		if (ret != TELLSTICK_SUCCESS) {
			return ret;
		}
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

int DeviceManager::setDeviceName(int deviceId, const std::wstring &name){

	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		int ret = d->set.setName(deviceId, name);
		if (ret != TELLSTICK_SUCCESS) {
			return ret;
		}
		it->second->setName(name);
	}
	else{
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}

	return TELLSTICK_SUCCESS;
}

std::wstring DeviceManager::getDeviceParameter(int deviceId, const std::wstring &name, const std::wstring &defaultValue){

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

int DeviceManager::setDeviceParameter(int deviceId, const std::wstring &name, const std::wstring &value)
{
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		int ret = d->set.setDeviceParameter(deviceId, name, value);
		if (ret != TELLSTICK_SUCCESS) {
			return ret;
		}
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

int DeviceManager::setDeviceProtocol(int deviceId, const std::wstring &protocol)
{
	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		int ret = d->set.setProtocol(deviceId, protocol);
		if (ret != TELLSTICK_SUCCESS) {
			return ret;
		}
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
	if(id < 0){
		return id;
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

	TelldusCore::MutexLocker deviceListLocker(&d->lock);
	if (!d->devices.size()) {
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}
	DeviceMap::iterator it = d->devices.find(deviceId);
	if (it != d->devices.end()) {
		TelldusCore::MutexLocker deviceLocker(it->second);
		return it->second->getType();
	}
	return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
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

void DeviceManager::connectTellStickController(int vid, int pid, const std::string &serial){
	d->controllerManager->deviceInsertedOrRemoved(vid, pid, serial, true);
}

void DeviceManager::disconnectTellStickController(int vid, int pid, const std::string &serial){
	d->controllerManager->deviceInsertedOrRemoved(vid, pid, serial, false);
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

	int retval = TELLSTICK_ERROR_UNKNOWN;

	if(device->getType() == TELLSTICK_TYPE_GROUP || device->getType() == TELLSTICK_TYPE_SCENE){
		std::wstring devices = device->getParameter(L"devices");
		deviceLocker = std::auto_ptr<TelldusCore::MutexLocker>(0);
		std::set<int> *duplicateDeviceIds = new std::set<int>;
		retval = doGroupAction(devices, action, data, device->getType(), deviceId, duplicateDeviceIds);
		delete duplicateDeviceIds;

		{
			//reaquire device lock, make sure it still exists
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
	}
	else{
		Controller *controller = d->controllerManager->getBestControllerById(device->getPreferredControllerId());
		if(!controller){
			Log::warning("Trying to execute action, but no controller found. Rescanning USB ports");
			//no controller found, scan for one, and retry once
			d->controllerManager->loadControllers();
			controller = d->controllerManager->getBestControllerById(device->getPreferredControllerId());
		}

		if(controller){
			retval = device->doAction(action, data, controller);
			if(retval == TELLSTICK_ERROR_BROKEN_PIPE){
				Log::warning("Error in communication with TellStick when executing action. Resetting USB");
				d->controllerManager->resetController(controller);
			}
			if(retval == TELLSTICK_ERROR_BROKEN_PIPE || retval == TELLSTICK_ERROR_NOT_FOUND){
				Log::warning("Rescanning USB ports");
				d->controllerManager->loadControllers();
				controller = d->controllerManager->getBestControllerById(device->getPreferredControllerId());
				if(!controller){
					Log::error("No contoller (TellStick) found, even after reset. Giving up.");
					return TELLSTICK_ERROR_NOT_FOUND;
				}
				retval = device->doAction(action, data, controller); //retry one more time
			}
		} else {
			Log::error("No contoller (TellStick) found after one retry. Giving up.");
			return TELLSTICK_ERROR_NOT_FOUND;
		}
	}
	if(retval == TELLSTICK_SUCCESS && device->getType() != TELLSTICK_TYPE_SCENE && device->getMethods() & action) {
		//if method isn't explicitly supported by device, but used anyway as a fallback (i.e. bell), don't change state
		std::wstring datastring = TelldusCore::charUnsignedToWstring(data);
		if (this->triggerDeviceStateChange(deviceId, action, datastring)) {
			device->setLastSentCommand(action, datastring);
			d->set.setDeviceState(deviceId, action, datastring);
		}
	}
	return retval;
}

int DeviceManager::doGroupAction(const std::wstring devices, const int action, const unsigned char data, const int type, const int groupDeviceId, std::set<int> *duplicateDeviceIds){
	int retval = TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
	std::wstring singledevice;
	std::wstringstream devicesstream(devices);

	duplicateDeviceIds->insert(groupDeviceId);

	while(std::getline(devicesstream, singledevice, L',')){

		int deviceId = TelldusCore::wideToInteger(singledevice);

		if(duplicateDeviceIds->count(deviceId) == 1){
			//action for device already executed, or will execute, do nothing to avoid infinite loop
			continue;
		}

		duplicateDeviceIds->insert(deviceId);

		int deviceReturnValue = TELLSTICK_SUCCESS;

		if(type == TELLSTICK_TYPE_SCENE && (action == TELLSTICK_TURNON || action == TELLSTICK_EXECUTE)){
			deviceReturnValue = executeScene(singledevice, groupDeviceId);
		}
		else if(type == TELLSTICK_TYPE_GROUP){
			if(deviceId != 0){
				int childType = DeviceManager::getDeviceType(deviceId);
				if(childType == TELLSTICK_TYPE_DEVICE){
					deviceReturnValue = doAction(deviceId, action, data);
				}
				else if(childType == TELLSTICK_TYPE_SCENE){
					deviceReturnValue = doGroupAction(DeviceManager::getDeviceParameter(deviceId, L"devices", L""), action, data, childType, deviceId, duplicateDeviceIds); //TODO make scenes infinite loops-safe
				}
				else{
					//group (in group)
					deviceReturnValue = doGroupAction(DeviceManager::getDeviceParameter(deviceId, L"devices", L""), action, data, childType, deviceId, duplicateDeviceIds);

					if(deviceReturnValue == TELLSTICK_SUCCESS) {
						std::wstring datastring = TelldusCore::charUnsignedToWstring(data);
						if (this->triggerDeviceStateChange(deviceId, action, datastring)) {
							DeviceManager::setDeviceLastSentCommand(deviceId, action, datastring);
							d->set.setDeviceState(deviceId, action, datastring);
						}
					}
				}
			}
			else{
				deviceReturnValue = TELLSTICK_ERROR_DEVICE_NOT_FOUND;	//Probably incorrectly formatted parameter
			}
		}

		if(deviceReturnValue != TELLSTICK_ERROR_METHOD_NOT_SUPPORTED){
			//if error(s), return the last error, but still try to continue the action with the other devices
			//if the error is a method not supported we igore is since there might be others supporting it
			//If no devices support the method the default value will be returned (method not supported)
			retval = deviceReturnValue;
		}

	}
	return retval;
}

int DeviceManager::executeScene(std::wstring singledevice, int groupDeviceId){

	std::wstringstream devicestream(singledevice);

	const int deviceParameterLength = 3;
	std::wstring deviceParts[deviceParameterLength] = {L"", L"", L""};
	std::wstring devicePart = L"";
	int i = 0;
	while(std::getline(devicestream, devicePart, L':') && i < deviceParameterLength){
		deviceParts[i] = devicePart;
		i++;
	}

	if(deviceParts[0] == L"" || deviceParts[1] == L""){
		return TELLSTICK_ERROR_UNKNOWN;	//malformed or missing parameter
	}

	int deviceId = TelldusCore::wideToInteger(deviceParts[0]);
	if(deviceId == groupDeviceId){
		return TELLSTICK_ERROR_UNKNOWN;	//the scene itself has been added to its devices, avoid infinite loop
	}
	int method = Device::methodId(TelldusCore::wideToString(deviceParts[1]));	//support methodparts both in the form of integers (e.g. TELLSTICK_TURNON) or text (e.g. "turnon")
	if(method == 0){
		method = TelldusCore::wideToInteger(deviceParts[1]);
	}
	unsigned char devicedata = 0;
	if(deviceParts[2] != L""){
		devicedata = TelldusCore::wideToInteger(deviceParts[2]);
	}

	if(deviceId > 0 && method > 0){	//check for format error in parameter "devices"
		return doAction(deviceId, method, devicedata);
	}

	return TELLSTICK_ERROR_UNKNOWN;
}

int DeviceManager::removeDevice(int deviceId){

	Device *device = 0;
	{
		int ret = d->set.removeDevice(deviceId);		//remove from register/settings
		if (ret != TELLSTICK_SUCCESS) {
			return ret;
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

	return TELLSTICK_SUCCESS;
}

std::wstring DeviceManager::getSensors() const {
	TelldusCore::MutexLocker sensorListLocker(&d->lock);

	TelldusCore::Message msg;

	msg.addArgument((int)d->sensorList.size());

	for (std::list<Sensor *>::iterator it = d->sensorList.begin(); it != d->sensorList.end(); ++it) {
		TelldusCore::MutexLocker sensorLocker(*it);
		msg.addArgument((*it)->protocol());
		msg.addArgument((*it)->model());
		msg.addArgument((*it)->id());
		msg.addArgument((*it)->dataTypes());
	}

	return msg;
}

std::wstring DeviceManager::getSensorValue(const std::wstring &protocol, const std::wstring &model, int id, int dataType) const {
	TelldusCore::MutexLocker sensorListLocker(&d->lock);
	Sensor *sensor = 0;
	for (std::list<Sensor *>::iterator it = d->sensorList.begin(); it != d->sensorList.end(); ++it) {
		TelldusCore::MutexLocker sensorLocker(*it);
		if (!TelldusCore::comparei((*it)->protocol(), protocol)) {
			continue;
		}
		if (!TelldusCore::comparei((*it)->model(), model)) {
			continue;
		}
		if ((*it)->id() != id) {
			continue;
		}
		sensor = *it;
		break;
	}

	if (!sensor) {
		return L"";
	}
	TelldusCore::MutexLocker sensorLocker(sensor);
	TelldusCore::Message msg;
	std::string value = sensor->value(dataType);
	if (value.length() > 0) {
		msg.addArgument(TelldusCore::charToWstring(value.c_str()));
		msg.addArgument((int)sensor->timestamp());
	}
	return msg;
}


void DeviceManager::handleControllerMessage(const ControllerEventData &eventData) {
	//Trigger raw-event
	EventUpdateData *eventUpdateData = new EventUpdateData();
	eventUpdateData->messageType = L"TDRawDeviceEvent";
	eventUpdateData->controllerId = eventData.controllerId;
	eventUpdateData->eventValue = TelldusCore::charToWstring(eventData.msg.c_str());
	d->deviceUpdateEvent->signal(eventUpdateData);

	ControllerMessage msg(eventData.msg);
	if (msg.msgClass().compare("sensor") == 0) {
		handleSensorMessage(msg);
		return;
	}

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
			it->second->setLastSentCommand(msg.method(), L"");
		}
	}
}

void DeviceManager::handleSensorMessage(const ControllerMessage &msg) {
	TelldusCore::MutexLocker sensorListLocker(&d->lock);
	Sensor *sensor = 0;
	for (std::list<Sensor *>::iterator it = d->sensorList.begin(); it != d->sensorList.end(); ++it) {
		TelldusCore::MutexLocker sensorLocker(*it);
		if (!TelldusCore::comparei((*it)->protocol(), msg.protocol())) {
			continue;
		}
		if (!TelldusCore::comparei((*it)->model(), msg.model())) {
			continue;
		}
		if ((*it)->id() != msg.getIntParameter("id")) {
			continue;
		}
		sensor = *it;
		break;
	}

	if (!sensor) {
		sensor = new Sensor(msg.protocol(), msg.model(), msg.getIntParameter("id"));
		d->sensorList.push_back(sensor);
	}
	TelldusCore::MutexLocker sensorLocker(sensor);

	time_t t = time(NULL);

	setSensorValueAndSignal("temp", TELLSTICK_TEMPERATURE, sensor, msg, t);
	setSensorValueAndSignal("humidity", TELLSTICK_HUMIDITY, sensor, msg, t);
}

void DeviceManager::setSensorValueAndSignal( const std::string &dataType, int dataTypeId, Sensor *sensor, const ControllerMessage &msg, time_t timestamp) const {
	if (!msg.hasParameter(dataType)) {
		return;
	}
	sensor->setValue(dataTypeId, msg.getParameter(dataType), timestamp);

	EventUpdateData *eventData = new EventUpdateData();
	eventData->messageType = L"TDSensorEvent";
	eventData->protocol = sensor->protocol();
	eventData->model = sensor->model();
	eventData->sensorId = sensor->id();
	eventData->dataType = dataTypeId;
	eventData->value = TelldusCore::charToWstring(sensor->value(dataTypeId).c_str());
	eventData->timestamp = (int)timestamp;
	d->deviceUpdateEvent->signal(eventData);
}

int DeviceManager::sendRawCommand(const std::wstring &command, int reserved){

	Controller *controller = d->controllerManager->getBestControllerById(-1);

	if(!controller){
		//no controller found, scan for one, and retry once
		d->controllerManager->loadControllers();
		controller = d->controllerManager->getBestControllerById(-1);
	}

	int retval = TELLSTICK_ERROR_UNKNOWN;
	if(controller){
		retval = controller->send(TelldusCore::wideToString(command));
		if(retval == TELLSTICK_ERROR_BROKEN_PIPE){
			d->controllerManager->resetController(controller);
		}
		if(retval == TELLSTICK_ERROR_BROKEN_PIPE || retval == TELLSTICK_ERROR_NOT_FOUND){
			d->controllerManager->loadControllers();
			controller = d->controllerManager->getBestControllerById(-1);
			if(!controller){
				return TELLSTICK_ERROR_NOT_FOUND;
			}
			retval = controller->send(TelldusCore::wideToString(command));  //retry one more time
		}
		return retval;
	} else {
		return TELLSTICK_ERROR_NOT_FOUND;
	}
}

bool DeviceManager::triggerDeviceStateChange(int deviceId, int intDeviceState, const std::wstring &strDeviceStateValue ) {
	if ( intDeviceState == TELLSTICK_BELL || intDeviceState == TELLSTICK_LEARN || intDeviceState == TELLSTICK_EXECUTE) {
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
