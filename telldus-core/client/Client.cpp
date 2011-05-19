#include "Client.h"
#include "CallbackDispatcher.h"
#include "Socket.h"
#include "Strings.h"
#include "Mutex.h"
#include "common.h"

#include <list>
#ifdef _WINDOWS
#include <memory>
#else
#include <tr1/memory>
#endif


using namespace TelldusCore;

typedef CallbackStruct<TDDeviceEvent> DeviceEvent;
typedef CallbackStruct<TDDeviceChangeEvent> DeviceChangeEvent;
typedef CallbackStruct<TDRawDeviceEvent> RawDeviceEvent;
typedef CallbackStruct<TDSensorEvent> SensorEvent;
typedef std::list<DeviceEvent *> DeviceEventList;
typedef std::list<DeviceChangeEvent *> DeviceChangeList;
typedef std::list<RawDeviceEvent *> RawDeviceEventList;
typedef std::list<SensorEvent *> SensorEventList;

class Client::PrivateData {
public:
	int lastCallbackId;
	DeviceEventList deviceEventList;
	DeviceChangeList deviceChangeEventList;
	Socket eventSocket;
	RawDeviceEventList rawDeviceEventList;
	SensorEventList sensorEventList;
	bool running, sensorCached;
	std::wstring sensorCache;
	TelldusCore::Mutex mutex;
};

Client *Client::instance = 0;

Client::Client()
	: Thread()
{
	d = new PrivateData;
	d->lastCallbackId = 0;
	d->running = true;
	d->sensorCached = false;
	start();
}

Client::~Client(void) {
	stopThread();
	wait();
	{
		TelldusCore::MutexLocker locker(&d->mutex);
	}
	delete d;
}

void Client::close() {
	if (Client::instance != 0) {
		delete Client::instance;
	}
}

Client *Client::getInstance() {
	if (Client::instance == 0) {
		Client::instance = new Client();
	}
	return Client::instance;
}

void Client::callbackDeviceEvent(int deviceId, int deviceState, const std::wstring &deviceStateValue){
	std::list<std::tr1::shared_ptr<TDDeviceEventDispatcher> > list;
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(DeviceEventList::iterator callback_it = d->deviceEventList.begin(); callback_it != d->deviceEventList.end(); ++callback_it) {
			std::tr1::shared_ptr<TDDeviceEventDispatcher> ptr(new TDDeviceEventDispatcher(*callback_it, deviceId, deviceState, TelldusCore::wideToString(deviceStateValue)));
			list.push_back(ptr);
		}
	}
}

void Client::callbackDeviceChangeEvent(int deviceId, int eventDeviceChanges, int eventChangeType){
	std::list<std::tr1::shared_ptr<TDDeviceChangeEventDispatcher> > list;
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(DeviceChangeList::iterator callback_it = d->deviceChangeEventList.begin(); callback_it != d->deviceChangeEventList.end(); ++callback_it) {
			std::tr1::shared_ptr<TDDeviceChangeEventDispatcher> ptr(new TDDeviceChangeEventDispatcher(*callback_it, deviceId, eventDeviceChanges, eventChangeType));
			list.push_back(ptr);
		}
	}
}

void Client::callbackRawEvent(std::wstring command, int controllerId) {
	std::list<std::tr1::shared_ptr<TDRawDeviceEventDispatcher> > list;
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(RawDeviceEventList::iterator callback_it = d->rawDeviceEventList.begin(); callback_it != d->rawDeviceEventList.end(); ++callback_it) {
			std::tr1::shared_ptr<TDRawDeviceEventDispatcher> ptr(new TDRawDeviceEventDispatcher(*callback_it, TelldusCore::wideToString(command), controllerId));
			list.push_back(ptr);
		}
	}
}

void Client::callbackSensorEvent(const std::wstring &protocol, const std::wstring &model, int id, int dataType, const std::wstring &value, int timestamp) {
	std::list<std::tr1::shared_ptr<TDSensorEventDispatcher> > list;
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(SensorEventList::iterator callback_it = d->sensorEventList.begin(); callback_it != d->sensorEventList.end(); ++callback_it) {
			std::tr1::shared_ptr<TDSensorEventDispatcher> ptr(new TDSensorEventDispatcher(*callback_it, TelldusCore::wideToString(protocol), TelldusCore::wideToString(model), id, dataType, TelldusCore::wideToString(value), timestamp));
			list.push_back(ptr);
		}
	}
}

bool Client::getBoolFromService(const Message &msg) {
	return getIntegerFromService(msg) == TELLSTICK_SUCCESS;
}

int Client::getIntegerFromService(const Message &msg) {
	std::wstring response = sendToService(msg);
	return Message::takeInt(&response);
}

std::wstring Client::getWStringFromService(const Message &msg) {
	std::wstring response = sendToService(msg);
	return Message::takeString(&response);
}

int Client::registerDeviceEvent( TDDeviceEvent eventFunction, void *context ) {
	TelldusCore::MutexLocker locker(&d->mutex);
	int id = ++d->lastCallbackId;
	DeviceEvent *callback = new DeviceEvent;
	callback->event = eventFunction;
	callback->id = id;
	callback->context = context;
	d->deviceEventList.push_back(callback);
	return id;
}

int Client::registerDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context ) {
	TelldusCore::MutexLocker locker(&d->mutex);
	int id = ++d->lastCallbackId;
	DeviceChangeEvent *callback = new DeviceChangeEvent;
	callback->event = eventFunction;
	callback->id = id;
	callback->context = context;
	d->deviceChangeEventList.push_back(callback);
	return id;
}

int Client::registerRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context ) {
	TelldusCore::MutexLocker locker(&d->mutex);
	int id = ++d->lastCallbackId;
	RawDeviceEvent *callback = new RawDeviceEvent;
	callback->event = eventFunction;
	callback->id = id;
	callback->context = context;
	d->rawDeviceEventList.push_back(callback);
	return id;
}

int Client::registerSensorEvent( TDSensorEvent eventFunction, void *context ) {
	TelldusCore::MutexLocker locker(&d->mutex);
	int id = ++d->lastCallbackId;
	SensorEvent *callback = new SensorEvent;
	callback->event = eventFunction;
	callback->id = id;
	callback->context = context;
	d->sensorEventList.push_back(callback);
	return id;
}

void Client::run(){
	//listen here
	d->eventSocket.connect(L"TelldusEvents");

	while(d->running){

		if(!d->eventSocket.isConnected()){
			d->eventSocket.connect(L"TelldusEvents");	//try to reconnect to service
			if(!d->eventSocket.isConnected()){
				//reconnect didn't succeed, wait a while and try again
				msleep(2000);
				continue;
			}
		}

		std::wstring clientMessage = d->eventSocket.read(5000);	//testing 5 second timeout
		while(clientMessage != L""){
			//a message arrived
			std::wstring type = Message::takeString(&clientMessage);
			if(type == L"TDDeviceChangeEvent"){
				int deviceId = Message::takeInt(&clientMessage);
				int eventDeviceChanges = Message::takeInt(&clientMessage);
				int eventChangeType = Message::takeInt(&clientMessage);
				callbackDeviceChangeEvent(deviceId, eventDeviceChanges, eventChangeType);
			}
			else if(type == L"TDDeviceEvent"){
				int deviceId = Message::takeInt(&clientMessage);
				int eventState = Message::takeInt(&clientMessage);
				std::wstring eventValue = Message::takeString(&clientMessage);
				callbackDeviceEvent(deviceId, eventState, eventValue);
			}
			else if(type == L"TDRawDeviceEvent"){
				std::wstring command = Message::takeString(&clientMessage);
				int controllerId = Message::takeInt(&clientMessage);
				callbackRawEvent(command, controllerId);
			}
			else if(type == L"TDSensorEvent"){
				std::wstring protocol = Message::takeString(&clientMessage);
				std::wstring model = Message::takeString(&clientMessage);
				int sensorId = Message::takeInt(&clientMessage);
				int dataType = Message::takeInt(&clientMessage);
				std::wstring value = Message::takeString(&clientMessage);
				int timestamp = Message::takeInt(&clientMessage);
				callbackSensorEvent(protocol, model, sensorId, dataType, value, timestamp);
			}
			else{
				clientMessage = L"";  //cleanup, if message contained garbage/unhandled data
			}
		}
	}
}

std::wstring Client::sendToService(const Message &msg) {
	Socket s;
	s.connect(L"TelldusClient");
	if (!s.isConnected()) { //Connection failed
		TelldusCore::Message msg;
		msg.addArgument(TELLSTICK_ERROR_CONNECTING_SERVICE);
		return msg;
	}
	s.write(msg.data());

	return s.read(5000);
}

void Client::stopThread(){
	d->running = false;
	d->eventSocket.stopReadWait();
}

bool Client::unregisterCallback( int callbackId ) {
	DeviceEventList newDEList;
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(DeviceEventList::iterator callback_it = d->deviceEventList.begin(); callback_it != d->deviceEventList.end(); ++callback_it) {
			if ( (*callback_it)->id != callbackId ) {
				continue;
			}
			newDEList.splice(newDEList.begin(), d->deviceEventList, callback_it);
			break;
		}
	}
	if (newDEList.size()) {
		DeviceEventList::iterator it = newDEList.begin();
		{ //Lock and unlock to make sure no one else uses the object
			TelldusCore::MutexLocker locker( &(*it)->mutex );
		}
		delete (*it);
		newDEList.erase(it);
		return true;
	}

	DeviceChangeList newDCList;
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(DeviceChangeList::iterator callback_it = d->deviceChangeEventList.begin(); callback_it != d->deviceChangeEventList.end(); ++callback_it) {
			if ( (*callback_it)->id != callbackId ) {
				continue;
			}
			newDCList.splice(newDCList.begin(), d->deviceChangeEventList, callback_it);
			break;
		}
	}
	if (newDCList.size()) {
		DeviceChangeList::iterator it = newDCList.begin();
		{TelldusCore::MutexLocker locker( &(*it)->mutex );}
		delete (*it);
		newDCList.erase(it);
		return true;
	}

	RawDeviceEventList newRDEList;
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(RawDeviceEventList::iterator callback_it = d->rawDeviceEventList.begin(); callback_it != d->rawDeviceEventList.end(); ++callback_it) {
			if ( (*callback_it)->id != callbackId ) {
				continue;
			}
			newRDEList.splice(newRDEList.begin(), d->rawDeviceEventList, callback_it );
			break;
		}
	}
	if (newRDEList.size()) {
		RawDeviceEventList::iterator it = newRDEList.begin();
		{TelldusCore::MutexLocker locker( &(*it)->mutex );}
		delete (*it);
		newRDEList.erase(it);
		return true;
	}

	SensorEventList newSEList;
	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(SensorEventList::iterator callback_it = d->sensorEventList.begin(); callback_it != d->sensorEventList.end(); ++callback_it) {
			if ( (*callback_it)->id != callbackId ) {
				continue;
			}
			newSEList.splice(newSEList.begin(), d->sensorEventList, callback_it );
			break;
		}
	}
	if (newSEList.size()) {
		SensorEventList::iterator it = newSEList.begin();
		{TelldusCore::MutexLocker locker( &(*it)->mutex );}
		delete (*it);
		newSEList.erase(it);
		return true;
	}

	return false;
}

int Client::getSensor(char *protocol, int protocolLen, char *model, int modelLen, int *sensorId, int *dataTypes) {
	if (!d->sensorCached) {
		Message msg(L"tdSensor");
		std::wstring response = Client::getWStringFromService(msg);
		int count = Message::takeInt(&response);
		d->sensorCached = true;
		d->sensorCache = L"";
		if (count > 0) {
			d->sensorCache = response;
		}
	}

	if (d->sensorCache == L"") {
		d->sensorCached = false;
		return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
	}

	std::wstring p = Message::takeString(&d->sensorCache);
	std::wstring m = Message::takeString(&d->sensorCache);
	int id = Message::takeInt(&d->sensorCache);
	int dt = Message::takeInt(&d->sensorCache);

	if (protocol && protocolLen) {
		strncpy(protocol, TelldusCore::wideToString(p).c_str(), protocolLen);
	}
	if (model && modelLen) {
		strncpy(model, TelldusCore::wideToString(m).c_str(), modelLen);
	}
	if (sensorId) {
		(*sensorId) = id;
	}
	if (dataTypes) {
		(*dataTypes) = dt;
	}

	return TELLSTICK_SUCCESS;
}
