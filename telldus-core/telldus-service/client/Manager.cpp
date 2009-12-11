
#include <QLocalSocket>
#include "Manager.h"
#include "Message.h"

#include <QFile>
#include <QTime>
void logMessage( const QString &message) {
	QFile file("C:/log.txt");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream out(&file);
	out << QTime::currentTime().toString() << ": " << message << "\n";
	file.close();
}

class ManagerPrivate {
public:
	int numberOfDevices;
	QLocalSocket s, eventSocket;
};

Manager *Manager::instance = 0;

Manager::Manager(void) {
	d = new ManagerPrivate;
	d->numberOfDevices = -1;
	connect(&d->eventSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

	d->s.connectToServer( "TelldusCoreClient" );
	d->eventSocket.connectToServer( "TelldusCoreEvents" );

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
	while(1) {
		QByteArray msg(d->eventSocket.readLine());
		if (msg.length() == 0) {
			break;
		}
		logMessage(Message::takeFirst(&msg).toString());
	}
	logMessage("Data-received");
}

QVariant Manager::send(const Message &message, const QVariant &default) {
	logMessage(QString("%1:").arg(QString(message)));
	if (d->s.state() != QLocalSocket::ConnectedState) {
		logMessage("[default]");
		return default;
	}
	d->s.write(message);
	if (d->s.waitForReadyRead(5000)) {
		QByteArray response(d->s.readLine());
		QVariant retval = Message::takeFirst(&response);
		logMessage(retval.toString());
		return retval;
	}
	logMessage("[No return]");
	return default;

}

