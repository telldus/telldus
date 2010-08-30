#include <QLocalSocket>
#include <libtelldus-core/telldus-core.h>
#include "Manager.h"
#include "Message.h"
#include "Socket.h"

#include "TelldusCore.h"

#include <QDebug>

using namespace TelldusService;

class ManagerPrivate {
public:
	Socket *s;
	bool running;
};

Manager::Manager(Socket *s, QObject *parent)
	:QThread(parent)
{
	//connect(this, SIGNAL(send(const Message &)), this, SLOT(sendMessage()));
	d = new ManagerPrivate;
	d->s = s;
	d->running = true;
	connect(this, SIGNAL(finished()), this, SIGNAL(done()));
//	connect(d->s, SIGNAL(dataArrived(const QByteArray &)), this, SLOT(dataArrived(const QByteArray &)));
	//TelldusCore::logMessage("  Manager created");
	this->start();
}

Manager::~Manager(void) {
	//TelldusCore::logMessage("  Destroying Manager");
	d->s->disconnect();
	//TelldusCore::logMessage("  Disconnected, waiting");
	wait();
	//TelldusCore::logMessage("  Waited, deleting");
	delete d->s;
	delete d;
	//TelldusCore::logMessage("  Manager destroyed");
}

QVariant Manager::parseMessage(const std::string &message) {
	std::string msg(message); //Copy
	std::string function(Message::takeString(&msg));
	if (function == "tdTurnOn") {
		int intDeviceId = Message::takeInt(&msg);
		return tdTurnOn(intDeviceId);

	} else if (function == "tdTurnOff") {
		int intDeviceId = Message::takeInt(&msg);
		return tdTurnOff(intDeviceId);

	} else if (function == "tdBell") {
		int intDeviceId = Message::takeInt(&msg);
		return tdBell(intDeviceId);

	} else if (function == "tdDim") {
		int intDeviceId = Message::takeInt(&msg);
		int level = Message::takeInt(&msg);
		return tdDim(intDeviceId, level);

	} else if (function == "tdLearn") {
		int intDeviceId = Message::takeInt(&msg);
		return tdLearn(intDeviceId);

	} else if (function == "tdLastSentCommand") {
		int intDeviceId = Message::takeInt(&msg);
		int methodsSupported = Message::takeInt(&msg);
		return tdLastSentCommand(intDeviceId, methodsSupported);

	} else if (function == "tdLastSentValue") {
		int intDeviceId = Message::takeInt(&msg);
		char *value = tdLastSentValue(intDeviceId);
		QByteArray retval(value);
		tdReleaseString(value);
		return retval;

	} else if (function == "tdGetNumberOfDevices") {
		return tdGetNumberOfDevices();

	} else if (function == "tdGetDeviceId") {
		int intDeviceIndex = Message::takeInt(&msg);
		return tdGetDeviceId(intDeviceIndex);

	} else if (function == "tdGetDeviceType") {
		int intDeviceId = Message::takeInt(&msg);
		return tdGetDeviceType(intDeviceId);

	} else if (function == "tdGetName") {
		int intDeviceId = Message::takeInt(&msg);
		char *name = tdGetName(intDeviceId);
		QByteArray retval(name);
		tdReleaseString(name);
		return retval;

	} else if (function == "tdSetName") {
		int intDeviceId = Message::takeInt(&msg);
		std::string name = Message::takeString(&msg);
		return tdSetName(intDeviceId, name.c_str());

	} else if (function == "tdGetProtocol") {
		int intDeviceId = Message::takeInt(&msg);
		char *protocol = tdGetProtocol(intDeviceId);
		QByteArray retval(protocol);
		tdReleaseString(protocol);
		return retval;

	} else if (function == "tdSetProtocol") {
		int intDeviceId = Message::takeInt(&msg);
		std::string protocol = Message::takeString(&msg);
		return tdSetProtocol(intDeviceId, protocol.c_str());

	} else if (function == "tdGetModel") {
		int intDeviceId = Message::takeInt(&msg);
		char *model = tdGetModel(intDeviceId);
		QByteArray retval(model);
		tdReleaseString(model);
		return retval;

	} else if (function == "tdSetModel") {
		int intDeviceId = Message::takeInt(&msg);
		std::string model = Message::takeString(&msg);
		return tdSetModel(intDeviceId, model.c_str());

	} else if (function == "tdSetDeviceParameter") {
		int intDeviceId = Message::takeInt(&msg);
		std::string name = Message::takeString(&msg);
		std::string value = Message::takeString(&msg);
		return tdSetDeviceParameter(intDeviceId, name.c_str(), value.c_str());

	} else if (function == "tdGetDeviceParameter") {
		int intDeviceId = Message::takeInt(&msg);
		std::string name = Message::takeString(&msg);
		std::string defaultValue = Message::takeString(&msg);
		char *value = tdGetDeviceParameter(intDeviceId, name.c_str(), defaultValue.c_str());
		QByteArray retval(value);
		tdReleaseString(value);
		return retval;

	} else if (function == "tdAddDevice") {
		return tdAddDevice();

	} else if (function == "tdRemoveDevice") {
		int intDeviceId = Message::takeInt(&msg);
		return tdRemoveDevice(intDeviceId);

	} else if (function == "tdMethods") {
		int intDeviceId = Message::takeInt(&msg);
		int intMethodsSupported = Message::takeInt(&msg);
		return tdMethods(intDeviceId, intMethodsSupported);

	} else if (function == "tdGetErrorString") {
		int intErrorNo = Message::takeInt(&msg);
		char *response = tdGetErrorString(intErrorNo);
		QByteArray retval(response);
		tdReleaseString(response);
		return retval;

	} else if (function == "tdSendRawCommand") {
		std::string command = Message::takeString(&msg);
		int reserved = Message::takeInt(&msg);
		return tdSendRawCommand(command.c_str(), reserved);
		
	} else if (function == "tdConnectTellStickController") {
		int vid = Message::takeInt(&msg);
		int pid = Message::takeInt(&msg);
		std::string serial = Message::takeString(&msg);
		tdConnectTellStickController(vid, pid, serial.c_str());
		return "";

	} else if (function == "tdDisconnectTellStickController") {
		int vid = Message::takeInt(&msg);
		int pid = Message::takeInt(&msg);
		std::string serial = Message::takeString(&msg);
		tdDisconnectTellStickController(vid, pid, serial.c_str());
		return "";

	}

	return 0;
}

void Manager::run() {
	while(1) {
		std::string data(d->s->read());
		if (data.length() == 0) {
			if (!d->s->connected()) {
				return;
			}
			continue;
		}
		//TelldusCore::logMessage(data);
		QVariant response(this->parseMessage(data));
		Message msg;
		if (response.type() == QVariant::Int) {
			msg.addArgument(response.toInt());
		} else {
			msg.addArgument(response.toString().toStdString());
		}
		msg.append("\n");
		d->s->write(msg);
	}
}
