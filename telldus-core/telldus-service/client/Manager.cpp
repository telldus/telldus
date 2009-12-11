
#include <QLocalSocket>
#include "Manager.h"
#include "Message.h"

class ManagerPrivate {
public:
	int numberOfDevices;
	QLocalSocket s;
};

Manager *Manager::instance = 0;

Manager::Manager(void) {
	d = new ManagerPrivate;
	d->numberOfDevices = -1;
	//connect(&d->s, SIGNAL(readyRead()), this, SLOT(dataReceived()));
	d->s.connectToServer( "TelldusCore" );
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
	if (d->numberOfDevices >= 0) {
		return d->numberOfDevices;
	}
	Message message("tdGetNumberOfDevices");
	d->numberOfDevices = this->send(message, 0).toInt();
	return d->numberOfDevices;
}

QString Manager::deviceName(int deviceId) {
	Message message("tdGetName");
	message.addArgument(deviceId);
	return this->send(message, "").toString();
}

void Manager::dataReceived() {
	
}

QVariant Manager::send(const Message &message, const QVariant &default) {
	if (d->s.state() != QLocalSocket::ConnectedState) {
		return default;
	}
	d->s.write(message);
	if (d->s.waitForReadyRead(1000)) {
		QVariant retval(d->s.readLine());
		return retval;
	}
	return default;

}

