#include "TelldusCore.h"
#include "Manager.h"
#include "Message.h"
#include "MessageReceiver.h"
#include <QLocalServer>
#include <QLocalSocket>

//Debug
#include <QDebug>
#include <QFile>
#include <QTime>

using namespace TelldusService;

class TelldusCorePrivate {
public:
	QLocalServer server, eventServer;
	QList<QLocalSocket *> eventSockets;
	MessageReceiver *messageReceiver;
};

TelldusCore::TelldusCore(void)
	:QObject(),
	d(new TelldusCorePrivate)
{
	connect(this, SIGNAL(deviceEventSignal(int, int, const char *)), this, SLOT(deviceEventSlot(int, int, const char *)));
	connect(this, SIGNAL(deviceChangeEventSignal(int, int, int)), this, SLOT(deviceChangeEventSlot(int, int, int)));

	tdInit();

	connect(&d->server, SIGNAL(newConnection()), this, SLOT(newConnection()));
#ifdef WINDOWS
	d->server.listen("TelldusCoreClient");
#else
	d->server.listen("/tmp/TelldusCoreClient");
	QFile socket(d->server.fullServerName());
	socket.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
#endif
	
	connect(&d->eventServer, SIGNAL(newConnection()), this, SLOT(newEventConnection()));
#ifdef WINDOWS
	d->eventServer.listen("TelldusCoreEvents");
#else
	d->eventServer.listen("/tmp/TelldusCoreEvents");
	QFile eventSocket(d->eventServer.fullServerName());
	eventSocket.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
#endif
	
	d->messageReceiver = new MessageReceiver(this);
	connect(d->messageReceiver, SIGNAL(deviceInserted(int,int,const QString &)), this, SLOT(deviceInserted(int,int,const QString &)));
	connect(d->messageReceiver, SIGNAL(deviceRemoved(int,int,const QString &)), this, SLOT(deviceRemoved(int,int,const QString &)));

	tdRegisterDeviceEvent( reinterpret_cast<TDDeviceEvent>(&deviceEvent), this);
	tdRegisterDeviceChangeEvent(reinterpret_cast<TDDeviceChangeEvent>(&deviceChangeEvent), this);
}

TelldusCore::~TelldusCore(void) {
	tdClose();
	logMessage("Shutting down");
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
	connect(m, SIGNAL(done()), this, SLOT(managerDone()));
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

void TelldusCore::deviceInserted(int vid, int pid, const QString &serial) {
	if (vid != 0x1781) {
		return;
	}
	if (pid == 0x0c30) {
		logMessage("TellStick Uno found " + serial);
	} else if (pid == 0x0c31) {
		logMessage("TellStick Duo found " + serial);
	}
	tdConnectTellStickController(vid, pid, serial.toLocal8Bit());
}

void TelldusCore::deviceRemoved(int vid, int pid, const QString &serial) {
	if (pid == 0x0c30) {
		logMessage("TellStick Uno disconnected " + serial);
	} else if (pid == 0x0c31) {
		logMessage("TellStick Duo disconnected " + serial);
	}
	tdDisconnectTellStickController(vid, pid, serial.toLocal8Bit());
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

void TelldusCore::deviceChangeEventSlot(int deviceId, int eventId, int changeType) {
	Message msg("TDDeviceChangeEvent");
	msg.addArgument(deviceId);
	msg.addArgument(eventId);
	msg.addArgument(changeType);
	foreach(QLocalSocket *s, d->eventSockets) {
		s->write(msg);
	}
}

void TelldusCore::logMessage( const QString &message) {
#ifdef _WINDOWS
	QFile file("C:/log.txt");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream out(&file);
	out << QTime::currentTime().toString() << ": " << message << "\n";
	file.close();
#else
	qDebug() << message;
#endif
}

void TelldusCore::logMessage( int message) {
	logMessage( QString::number(message) );
}

void TelldusCore::logWinError( int errorNo) {
	logMessage("WinError:");
	logMessage(errorNo);
	/*LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorNo,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );
	logMessage( QString(reinterpret_cast<char *>(lpMsgBuf)) );*/
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

void WINAPI TelldusCore::deviceChangeEvent(int deviceId, int eventId, int changeType, int, void *context) {
	TelldusCore *tc = reinterpret_cast<TelldusCore *>(context);
	if (tc) {
		emit tc->deviceChangeEventSignal(deviceId, eventId, changeType);
	}
}

