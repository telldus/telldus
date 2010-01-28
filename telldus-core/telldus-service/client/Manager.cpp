
#include <QLocalSocket>
#include "Manager.H"
#include "Message.h"

typedef std::list<CallbackStruct> CallbackList;
typedef std::list<DeviceChangeCallbackStruct> DeviceChangeCallbackList;
typedef std::list<RawCallbackStruct> RawCallbackList;

class ManagerPrivate {
public:
	int numberOfDevices;
	int lastCallbackId;
	QLocalSocket s, eventSocket;
	CallbackList callbacks;
	DeviceChangeCallbackList deviceChangeCallbacks;
	RawCallbackList rawCallbacks;
};

Manager *Manager::instance = 0;

Manager::Manager(void) {
	d = new ManagerPrivate;
	d->numberOfDevices = -1;
	d->lastCallbackId = 0;
	connect(&d->eventSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

#ifdef _WINDOWS
	d->s.connectToServer( "TelldusCoreClient" );
	d->eventSocket.connectToServer( "TelldusCoreEvents" );
#else
	d->s.connectToServer( "/tmp/TelldusCoreClient" );
	d->eventSocket.connectToServer( "/tmp/TelldusCoreEvents" );
#endif

	d->s.waitForConnected();
}

Manager::~Manager(void) {
	delete d;
}

Manager *Manager::getInstance() {
	if (Manager::instance == 0) {
		Manager::instance = new Manager();
	}
	return Manager::instance;
}


void Manager::close() {
	if (Manager::instance != 0) {
		delete Manager::instance;
	}
}

int Manager::numberOfDevices() {
	bool ok;
	if (d->numberOfDevices >= 0) {
		return d->numberOfDevices;
	}
	Message message("tdGetNumberOfDevices");
	d->numberOfDevices = this->send(message, &ok).toInt();
	return d->numberOfDevices;
}

QString Manager::deviceName(int deviceId) {
	bool ok;
	Message message("tdGetName");
	message.addArgument(deviceId);
	QString retval = this->send(message, &ok).toString();
	if (!ok) {
		retval = "";
	}
	return retval;
}

int Manager::registerDeviceEvent( TDDeviceEvent eventFunction, void *context ) {
	int id = ++d->lastCallbackId;
	CallbackStruct callback = {eventFunction, id, context};
	d->callbacks.push_back(callback);
	return id;
}

int Manager::registerDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context ) {
	int id = ++d->lastCallbackId;
	DeviceChangeCallbackStruct callback = {eventFunction, id, context};
	d->deviceChangeCallbacks.push_back(callback);
	return id;
}

int Manager::registerRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context ) {
	int id = ++d->lastCallbackId;
	RawCallbackStruct callback = {eventFunction, id, context};
	d->rawCallbacks.push_back(callback);
	return id;
}


void Manager::dataReceived() {
	while(1) {
		QByteArray msg(d->eventSocket.readLine());
		if (msg.length() == 0) {
			break;
		}
		QString funcName = Message::takeFirst(&msg).toString();
		if (funcName == "TDDeviceEvent") {
			int intDeviceId = Message::takeFirst(&msg).toInt();
			int intDeviceState = Message::takeFirst(&msg).toInt();
			QString strDeviceStateValue = Message::takeFirst(&msg).toString();
			for(CallbackList::const_iterator callback_it = d->callbacks.begin(); callback_it != d->callbacks.end(); ++callback_it) {
				(*callback_it).event(intDeviceId, intDeviceState, strDeviceStateValue.toLocal8Bit(), (*callback_it).id, (*callback_it).context);
			}
		} else if (funcName == "TDDeviceChangeEvent") {
			int intDeviceId = Message::takeFirst(&msg).toInt();
			int intEvent = Message::takeFirst(&msg).toInt();
			int intChange = Message::takeFirst(&msg).toInt();
			//Clear the cache
			d->numberOfDevices = -1;
			for(DeviceChangeCallbackList::const_iterator callback_it = d->deviceChangeCallbacks.begin(); callback_it != d->deviceChangeCallbacks.end(); ++callback_it) {
				(*callback_it).event(intDeviceId, intEvent, intChange, (*callback_it).id, (*callback_it).context);
			}
		}
	}
}

QVariant Manager::send(const Message &message, bool *success) {
	(*success) = false;
	if (d->s.state() != QLocalSocket::ConnectedState) {
		return TELLSTICK_ERROR_CONNECTING_SERVICE;
	}
	d->s.write(message);
	if (d->s.waitForReadyRead(7000)) {
		QByteArray response(d->s.readLine());
		QVariant retval = Message::takeFirst(&response);
		(*success) = true;
		return retval;
	}
	return TELLSTICK_ERROR_UNKNOWN_RESPONSE;

}

