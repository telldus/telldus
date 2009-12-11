
#include <QLocalSocket>
#include <QMutex>
#include <libtelldus-core/telldus-core.h>
#include "Manager.h"
#include "Message.h"

#include "TelldusCore.h"

class ManagerPrivate {
public:
	QLocalSocket *s;
};

Manager::Manager(QLocalSocket *s, QObject *parent)
	:QThread(parent)
{
	d = new ManagerPrivate;
	d->s = s;
	TelldusCore::logMessage("  Manager created");
	this->start();
}

Manager::~Manager(void) {
	delete d;
	TelldusCore::logMessage("  Manager destroyed");
}

void Manager::run() {
	TelldusCore::logMessage("  Started loop");
	while(d->s->state() == QLocalSocket::ConnectedState) {
		if (d->s->waitForReadyRead()) {
			QVariant response(this->parseMessage(d->s->readLine()));
			Message msg;
			msg.addArgument(response);
			msg.append("\n");
			d->s->write(msg);
		}
	}
	TelldusCore::logMessage("  loop ended");
}

QVariant Manager::parseMessage(const QByteArray &message) {
	QByteArray msg = message; //Copy
	QVariant function(Message::takeFirst(&msg));
	if (function == "tdTurnOn") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		return tdTurnOn(intDeviceId);

	} else if (function == "tdTurnOff") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		return tdTurnOff(intDeviceId);

	} else if (function == "tdBell") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		return tdBell(intDeviceId);

	} else if (function == "tdDim") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		int level = Message::takeFirst(&msg).toInt();
		return tdDim(intDeviceId, level);

	} else if (function == "tdLearn") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		return tdLearn(intDeviceId);

	} else if (function == "tdLastSentCommand") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		int methodsSupported = Message::takeFirst(&msg).toInt();
		return tdLastSentCommand(intDeviceId, methodsSupported);

	} else if (function == "tdLastSentValue") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		char *value = tdLastSentValue(intDeviceId);
		QByteArray retval(value);
		tdReleaseString(value);
		return retval;

	} else if (function == "tdGetNumberOfDevices") {
		return tdGetNumberOfDevices();

	} else if (function == "tdGetDeviceId") {
		int intDeviceIndex = Message::takeFirst(&msg).toInt();
		return tdGetDeviceId(intDeviceIndex);

	} else if (function == "tdGetDeviceType") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		return tdGetDeviceType(intDeviceId);

	} else if (function == "tdGetName") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		char *name = tdGetName(intDeviceId);
		QByteArray retval(name);
		tdReleaseString(name);
		return retval;

	} else if (function == "tdSetName") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		QString name = Message::takeFirst(&msg).toString();
		return tdSetName(intDeviceId, name.toLocal8Bit());

	} else if (function == "tdGetProtocol") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		char *protocol = tdGetProtocol(intDeviceId);
		QByteArray retval(protocol);
		tdReleaseString(protocol);
		return retval;

	} else if (function == "tdSetProtocol") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		QString protocol = Message::takeFirst(&msg).toString();
		return tdSetProtocol(intDeviceId, protocol.toLocal8Bit());

	} else if (function == "tdGetModel") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		char *model = tdGetModel(intDeviceId);
		QByteArray retval(model);
		tdReleaseString(model);
		return retval;

	} else if (function == "tdSetModel") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		QString model = Message::takeFirst(&msg).toString();
		return tdSetModel(intDeviceId, model.toLocal8Bit());

	} else if (function == "tdSetDeviceParameter") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		QString name = Message::takeFirst(&msg).toString();
		QString value = Message::takeFirst(&msg).toString();
		return tdSetDeviceParameter(intDeviceId, name.toLocal8Bit(), value.toLocal8Bit());

	} else if (function == "tdGetDeviceParameter") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		QString name = Message::takeFirst(&msg).toString();
		QString defaultValue = Message::takeFirst(&msg).toString();
		char *value = tdGetDeviceParameter(intDeviceId, name.toLocal8Bit(), defaultValue.toLocal8Bit());
		QByteArray retval(value);
		tdReleaseString(value);
		return retval;

	} else if (function == "tdAddDevice") {
		return tdAddDevice();

	} else if (function == "tdRemoveDevice") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		return tdRemoveDevice(intDeviceId);

	} else if (function == "tdMethods") {
		int intDeviceId = Message::takeFirst(&msg).toInt();
		int intMethodsSupported = Message::takeFirst(&msg).toInt();
		return tdMethods(intDeviceId, intMethodsSupported);

	} else if (function == "tdGetErrorString") {
		int intErrorNo = Message::takeFirst(&msg).toInt();
		char *response = tdGetErrorString(intErrorNo);
		QByteArray retval(response);
		tdReleaseString(response);
		return retval;

	} else if (function == "tdSendRawCommand") {
		QString command = Message::takeFirst(&msg).toString();
		int reserved = Message::takeFirst(&msg).toInt();
		return tdSendRawCommand(command.toLocal8Bit(), reserved);

	}

	return 0;
}