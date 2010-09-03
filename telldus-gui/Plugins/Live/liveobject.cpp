#include "liveobject.h"
#include <QtNetwork>
#include <QtCore>
#include <QDesktopServices>

#include <QDebug>

class LiveObject::PrivateData {
public:
	QTcpSocket *socket;
	QTimer timer;
	bool registered;
	QString mac;
};

LiveObject::LiveObject( QScriptEngine *engine, QObject * parent )
		: QObject(parent)
{
	d = new PrivateData;
	d->registered = false;
	d->socket = new QTcpSocket(this);
	connect(d->socket, SIGNAL(connected()), this, SLOT(p_connected()));
	connect(d->socket, SIGNAL(disconnected()), this, SLOT(p_disconnected()));
	connect(d->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(d->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	connect(d->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));

	d->timer.setInterval(60000); //Once a minute
	connect(&d->timer, SIGNAL(timeout()), this, SLOT(pingServer()));
}

LiveObject::~LiveObject() {
	delete d;
}

void LiveObject::activate() {
	if (d->mac == "" || d->socket->state() != QAbstractSocket::ConnectedState) {
		return;
	}
	QDesktopServices::openUrl(QUrl("http://stage.telldus.se/telldus-live/register/client?address=" + d->mac));
}

void LiveObject::connectToServer() {
	d->socket->abort();
	d->socket->connectToHost("tech.telldus.com", 50000);
}

void LiveObject::disconnect() {
	d->socket->disconnectFromHost();
}

void LiveObject::readyRead() {
	QByteArray ba = d->socket->readAll();
	LiveMessage msg = LiveMessage::fromByteArray(ba);
	if (msg.name() == "") {
		return;
	} else if (msg.name() == "registered") {
		d->registered = true;
		emit registered();
	} else {
		emit messageReceived(&msg);
	}
}

void LiveObject::pingServer() {
	if (d->socket->state() != QAbstractSocket::ConnectedState) {
		return;
	}
	LiveMessage msg("Ping");
	this->sendMessage(msg);
}


void LiveObject::sendMessage(const LiveMessage &message) {
	d->socket->write(message.toByteArray());
	d->socket->flush();
}

void LiveObject::sendMessage(LiveMessage *message) {
	if (!d->registered) {
		return;
	}
	this->sendMessage(*message);
}

void LiveObject::p_connected() {
	//Lets find out our mac-address
	QNetworkInterface iface = interfaceFromAddress(d->socket->localAddress());
	if (!iface.isValid()) {
		return;
	}
	d->timer.start();
	d->mac = iface.hardwareAddress().replace(":","");
	LiveMessage msg("Register");
	msg.append(d->mac);
	this->sendMessage(msg);
	emit connected();
}

void LiveObject::p_disconnected() {
	d->timer.stop();
}

void LiveObject::error( QAbstractSocket::SocketError socketError ) {
}

void LiveObject::stateChanged( QAbstractSocket::SocketState socketState ) {
}

QNetworkInterface LiveObject::interfaceFromAddress( const QHostAddress &address ) {
	QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
	foreach (QNetworkInterface i, interfaceList) {
		foreach(QNetworkAddressEntry a, i.addressEntries()) {
			if(a.ip() == address) {
				return i;
			}
		}
	}
	return QNetworkInterface();
}
