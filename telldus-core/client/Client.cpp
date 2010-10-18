#include "Client.h"
#include "Socket.h"

#include <list>

using namespace TelldusCore;

template <typename T> struct CallbackStruct {
	T event;
	int id;
	void *context;
};
typedef CallbackStruct<TDDeviceEvent> DeviceEvent;
typedef CallbackStruct<TDDeviceChangeEvent> DeviceChangeEvent;
typedef CallbackStruct<TDRawDeviceEvent> RawDeviceEvent;
typedef std::list<DeviceEvent> DeviceEventList;
typedef std::list<DeviceChangeEvent> DeviceChangeList;
typedef std::list<RawDeviceEvent> RawDeviceEventList;

class Client::PrivateData {
public:
	Socket eventSocket;
	int lastCallbackId;
	DeviceEventList deviceEventList;
	DeviceChangeList deviceChangeEventList;
	RawDeviceEventList rawDeviceEventList;
};

Client *Client::instance = 0;

Client::Client() {
	d = new PrivateData;
	d->lastCallbackId = 0;
	d->eventSocket.connect(L"TelldusEvents");
}

Client::~Client(void) {
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

bool Client::getBoolFromService(const Message &msg) {
	return getIntegerFromService(msg) == 1;
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
	int id = ++d->lastCallbackId;
	DeviceEvent callback = {eventFunction, id, context};
	d->deviceEventList.push_back(callback);
	return id;
}

int Client::registerDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context ) {
	int id = ++d->lastCallbackId;
	DeviceChangeEvent callback = {eventFunction, id, context};
	d->deviceChangeEventList.push_back(callback);
	return id;
}

int Client::registerRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context ) {
	int id = ++d->lastCallbackId;
	RawDeviceEvent callback = {eventFunction, id, context};
	d->rawDeviceEventList.push_back(callback);
	return id;
}

std::wstring Client::sendToService(const Message &msg) {
	Socket s;
	s.connect(L"TelldusClient");
	s.write(msg.data());

	return s.read();
}

bool Client::unregisterCallback( int callbackId ) {
	for(DeviceEventList::iterator callback_it = d->deviceEventList.begin(); callback_it != d->deviceEventList.end(); ++callback_it) {
		if ( (*callback_it).id != callbackId ) {
			continue;
		}
		d->deviceEventList.erase(callback_it);
		return true;
	}
	
	for(DeviceChangeList::iterator callback_it = d->deviceChangeEventList.begin(); callback_it != d->deviceChangeEventList.end(); ++callback_it) {
		if ( (*callback_it).id != callbackId ) {
			continue;
		}
		d->deviceChangeEventList.erase(callback_it);
		return true;
	}
	
	for(RawDeviceEventList::iterator callback_it = d->rawDeviceEventList.begin(); callback_it != d->rawDeviceEventList.end(); ++callback_it) {
		if ( (*callback_it).id != callbackId ) {
			continue;
		}
		d->rawDeviceEventList.erase(callback_it);
		return true;
	}
	
	return false;
}