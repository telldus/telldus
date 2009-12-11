#include "TelldusCore.h"
#include "Manager.h"
#include "Message.h"
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>

class TelldusCorePrivate {
public:
	QLocalServer server, eventServer;
	QList<QLocalSocket *> eventSockets;
};

TelldusCore::TelldusCore(void)
	:QObject(),
	d(new TelldusCorePrivate)
{
	connect(this, SIGNAL(deviceEventSignal(int, int, const char *)), this, SLOT(deviceEventSlot(int, int, const char *)));

	tdInit();

	connect(&d->server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	d->server.listen("TelldusCoreClient");

	connect(&d->eventServer, SIGNAL(newConnection()), this, SLOT(newEventConnection()));
	d->eventServer.listen("TelldusCoreEvents");

	tdRegisterDeviceEvent( reinterpret_cast<TDDeviceEvent>(&deviceEvent), this);
}

TelldusCore::~TelldusCore(void) {
	tdClose();
	delete d;
}

void TelldusCore::managerDone() {
	//Cleanup our manager after us
	Manager *m = qobject_cast<Manager *>(sender());
	if (m) {
		delete m;
	}
}

void TelldusCore::newConnection() {
	logMessage(" New normal Connection");
	QLocalSocket *s = d->server.nextPendingConnection();
	Manager *m = new Manager(s, this);
	connect(m, SIGNAL(finished()), this, SLOT(managerDone()));
}

void TelldusCore::newEventConnection() {
	logMessage(" New eventConnection");
	QLocalSocket *s = d->eventServer.nextPendingConnection();
	connect(s, SIGNAL(disconnected()), this, SLOT(disconnected()));
	d->eventSockets.append(s);
}

void TelldusCore::disconnected() {
	logMessage(" Event disconnection");
	QLocalSocket *s = qobject_cast<QLocalSocket *>(sender());
	if (s) {
		d->eventSockets.removeOne(s);
		//if (d->eventServer.isListening()) {
			//delete s;
		//}
	}
}

void TelldusCore::deviceEventSlot(int deviceId, int method, const char *data) {
	Message msg("TDDeviceEvent");
	msg.addArgument(deviceId);
	msg.addArgument(method);
	msg.addArgument(data);
	foreach(QLocalSocket *s, d->eventSockets) {
		logMessage(" Sending deviceEvent");
		s->write(msg);
	}
}

#include <QFile>
#include <QTime>

void TelldusCore::logMessage( const QString &message) {
	QFile file("C:/log.txt");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream out(&file);
	//out << QTime::currentTime().toString() << ": " << message << "\n";
	file.close();
}

void WINAPI TelldusCore::deviceEvent(int deviceId, int method, const char *data, int, void *context) {
	TelldusCore *tc = reinterpret_cast<TelldusCore *>(context);
	if (tc) {
		//The reason we emit the signal here and don't just call the function is because this
		//deviceEvent could be called by any thread. Qts signals makes sure our normal thread
		//runs the function instead.
		emit tc->deviceEventSignal(deviceId, method, data);
	}
}
