
#include "Manager.h"
#include "Message.h"
#include "../common/Socket.h"
#include <list>

using namespace TelldusService;

typedef std::list<CallbackStruct> CallbackList;
typedef std::list<DeviceChangeCallbackStruct> DeviceChangeCallbackList;
typedef std::list<RawCallbackStruct> RawCallbackList;

class ManagerPrivate {
public:
	int numberOfDevices;
	int lastCallbackId;
	Socket s, eventSocket;
	MUTEX sMutex;
	CallbackList callbacks;
	DeviceChangeCallbackList deviceChangeCallbacks;
	RawCallbackList rawCallbacks;
};

Manager *Manager::instance = 0;

Manager::Manager(void)
	:Thread()
{
	d = new ManagerPrivate;
	Thread::initMutex(&d->sMutex);
	d->numberOfDevices = -1;
	d->lastCallbackId = 0;

#ifdef _WINDOWS
	d->s.connectToServer( "TelldusCoreClient" );
	d->eventSocket.connectToServer( "TelldusCoreEvents" );
#else
	d->s.connectToServer( "/tmp/TelldusCoreClient" );
	d->eventSocket.connectToServer( "/tmp/TelldusCoreEvents" );
#endif

	start();
}

Manager::~Manager(void) {
	d->eventSocket.disconnect();
	this->wait();
	Thread::destroyMutex(&d->sMutex);
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
	d->numberOfDevices = this->sendAndReceiveInt(message, &ok);
	return d->numberOfDevices;
}

std::string Manager::deviceName(int deviceId) {
	bool ok;
	Message message("tdGetName");
	message.addArgument(deviceId);
	std::string retval = this->sendAndReceiveString(message, &ok);
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
		logMessage("-> OVERLAPPED read");
		std::string data(d->eventSocket.readOverlapped());
//		logMessage(QString("<- OVERLAPPED result: %1").arg(QString::fromStdString(data)));
		if (data.length() == 0) {
			continue;
		}
		this->dataReceived(data);
	}
}

void Manager::dataReceived(const std::string &message) {
	std::string msg(message); //Copy
//	logMessage(QString::fromStdString(msg));
	std::string funcName = Message::takeString(&msg);
//	logMessage(QString::fromStdString(funcName));
	if (funcName == "TDDeviceEvent") {
		int intDeviceId = Message::takeInt(&msg);
		int intDeviceState = Message::takeInt(&msg);
		std::string strDeviceStateValue = Message::takeString(&msg);
//		logMessage(QString("Sending %1 callbacks").arg(d->callbacks.size()));
		for(CallbackList::const_iterator callback_it = d->callbacks.begin(); callback_it != d->callbacks.end(); ++callback_it) {
			logMessage("StartSend");
			(*callback_it).event(intDeviceId, intDeviceState, strDeviceStateValue.c_str(), (*callback_it).id, (*callback_it).context);
			logMessage("SendDone");
		}
	} else if (funcName == "TDDeviceChangeEvent") {
		int intDeviceId = Message::takeInt(&msg);
		int intEvent = Message::takeInt(&msg);
		int intChange = Message::takeInt(&msg);
		//Clear the cache
		d->numberOfDevices = -1;
//		logMessage(QString("Number of callbacks %1").arg(d->deviceChangeCallbacks.size()));
		for(DeviceChangeCallbackList::const_iterator callback_it = d->deviceChangeCallbacks.begin(); callback_it != d->deviceChangeCallbacks.end(); ++callback_it) {
			(*callback_it).event(intDeviceId, intEvent, intChange, (*callback_it).id, (*callback_it).context);
		}
	} else if (funcName == "TDRawDeviceEvent") {
		std::string strData = Message::takeString(&msg);
		int controllerId = Message::takeInt(&msg);
		for(RawCallbackList::const_iterator callback_it = d->rawCallbacks.begin(); callback_it != d->rawCallbacks.end(); ++callback_it) {
			(*callback_it).event(strData.c_str(), controllerId, (*callback_it).id, (*callback_it).context);
		}
	}
}

std::string Manager::send(const Message &message, bool *success) {
	(*success) = false;
	if (!d->s.connected()) {
		Message msg;
		msg.addArgument(TELLSTICK_ERROR_CONNECTING_SERVICE);
		return msg;
	}
	std::string response;
	{
		TelldusCore::MutexLocker l(&d->sMutex);
//		logMessage(QString("Sending: %1").arg(QString::fromStdString(message)));
		response = d->s.readWriteOverlapped(message);
	}
	if (response.length() > 0) {
//		logMessage(QString("Received: %1 from: %2").arg(QString::fromStdString(response).trimmed()).arg(QString::fromStdString(message)));
		(*success) = true;
		return response;
	}
	logMessage("Received: empty result");
	Message msg;
	msg.addArgument(TELLSTICK_ERROR_UNKNOWN_RESPONSE);
	return msg;
}

std::string Manager::sendAndReceiveString(const TelldusService::Message &msg, bool *success) {
	std::string message(send(msg, success));
	return Message::takeString(&message);
}

int Manager::sendAndReceiveInt(const TelldusService::Message &msg, bool *success) {
	std::string message(send(msg, success));
	return Message::takeInt(&message);
}

//#include <QFile>
//#include <QTextStream>
//#include <QTime>
//#include <QDebug>
//void Manager::logMessage( const QString &message) {
/*#ifdef _WINDOWS
	QFile file("C:/log_client.txt");
	return;
	static bool firstRun = true;
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
*/

void Manager::logMessage( const std::string &message) {
}