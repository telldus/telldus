
#include "Manager.h"
#include "Message.h"
#include "../common/Socket.h"

using namespace TelldusService;

typedef std::list<CallbackStruct> CallbackList;
typedef std::list<DeviceChangeCallbackStruct> DeviceChangeCallbackList;
typedef std::list<RawCallbackStruct> RawCallbackList;

class ManagerPrivate {
public:
	int numberOfDevices;
	int lastCallbackId;
	Socket s, eventSocket;
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

	//d->s.waitForConnected();
	start();
}

Manager::~Manager(void) {
	d->eventSocket.disconnect();
	this->wait();
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

bool Manager::unregisterCallback( int callbackId ) {
	for(CallbackList::iterator callback_it = d->callbacks.begin(); callback_it != d->callbacks.end(); ++callback_it) {
		if ( (*callback_it).id != callbackId ) {
			continue;
		}
		d->callbacks.erase(callback_it);
		return true;
	}

	for(DeviceChangeCallbackList::iterator callback_it = d->deviceChangeCallbacks.begin(); callback_it != d->deviceChangeCallbacks.end(); ++callback_it) {
		if ( (*callback_it).id != callbackId ) {
			continue;
		}
		d->deviceChangeCallbacks.erase(callback_it);
		return true;
	}

	for(RawCallbackList::iterator callback_it = d->rawCallbacks.begin(); callback_it != d->rawCallbacks.end(); ++callback_it) {
		if ( (*callback_it).id != callbackId ) {
			continue;
		}
		d->rawCallbacks.erase(callback_it);
		return true;
	}

	return false;
}

void Manager::run() {
	while( d->eventSocket.connected() ) {
		logMessage("-- OVERLAPPED read");
		QByteArray data(d->eventSocket.readOverlapped());
		logMessage("-- OVERLAPPED result:");
		logMessage(data);
		if (data.length() == 0) {
			continue;
		}
		this->dataReceived(data);
	}
}

void Manager::dataReceived(const QByteArray &message) {
	QByteArray msg(message); //Copy
	logMessage("Event");
	logMessage(msg);
	QString funcName = Message::takeFirst(&msg).toString();
	logMessage(funcName);
	if (funcName == "TDDeviceEvent") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		int intDeviceState = Message::takeFirst(&msg).toInt();
		QString strDeviceStateValue = Message::takeFirst(&msg).toString();
		logMessage(QString("Sending %1 callbacks").arg(d->callbacks.size()));
		for(CallbackList::const_iterator callback_it = d->callbacks.begin(); callback_it != d->callbacks.end(); ++callback_it) {
			logMessage("StartSend");
			(*callback_it).event(intDeviceId, intDeviceState, strDeviceStateValue.toLocal8Bit(), (*callback_it).id, (*callback_it).context);
			logMessage("SendDone");
		}
	} else if (funcName == "TDDeviceChangeEvent") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		int intEvent = Message::takeFirst(&msg).toInt();
		int intChange = Message::takeFirst(&msg).toInt();
		//Clear the cache
		d->numberOfDevices = -1;
		logMessage(QString("Number of callbacks %1").arg(d->deviceChangeCallbacks.size()));
		for(DeviceChangeCallbackList::const_iterator callback_it = d->deviceChangeCallbacks.begin(); callback_it != d->deviceChangeCallbacks.end(); ++callback_it) {
			(*callback_it).event(intDeviceId, intEvent, intChange, (*callback_it).id, (*callback_it).context);
		}
	}
}

QVariant Manager::send(const Message &message, bool *success) {
	(*success) = false;
	if (!d->s.connected()) {
		return TELLSTICK_ERROR_CONNECTING_SERVICE;
	}
	logMessage(QString("Sending: %1").arg(QString(message)));
	d->s.writeOverlapped(message);
	QByteArray response(d->s.readOverlapped());
	logMessage(QString("Received: %1").arg(QString(response)));
	if (response.length() > 0) {
		QVariant retval = Message::takeFirst(&response);
		(*success) = true;
		return retval;
	}
	return TELLSTICK_ERROR_UNKNOWN_RESPONSE;
}

#include <QFile>
#include <QTextStream>
#include <QTime>
void Manager::logMessage( const QString &message) {
	//return;
#ifdef _WINDOWS
	static bool firstRun = true;
	QFile file("C:/log_client.txt");
	if (firstRun) {
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		firstRun = false;
	} else {
		file.open(QIODevice::Append | QIODevice::Text);
	}
	QTextStream out(&file);
	out << QTime::currentTime().toString() << ": " << message << "\n";
	file.close();
#else
	qDebug() << message;
#endif
}
