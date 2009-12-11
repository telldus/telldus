#include "TelldusCore.h"
#include "Manager.h"
#include <libtelldus-core/telldus-core.h>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>

void WINAPI deviceEvent(int deviceId, int method, const char *, int, void *) {
	qDebug() << "DeviceEvent" << deviceId << method;
}

class TelldusCorePrivate {
public:
	QLocalServer server;
};

TelldusCore::TelldusCore(void)
	:QObject(),
	d(new TelldusCorePrivate)
{
	tdInit();

	connect(&d->server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	d->server.listen("TelldusCore");

	tdRegisterDeviceEvent( reinterpret_cast<TDDeviceEvent>(&deviceEvent), 0);
}

TelldusCore::~TelldusCore(void) {
	tdClose();
	delete d;
}

void TelldusCore::newConnection() {
	QLocalSocket *s = d->server.nextPendingConnection();
	Manager *m = new Manager(s, this);
}