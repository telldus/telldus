#include "liveobject.h"
#include "config.h"
#include <QtNetwork>
#include <QtCore>
#include <QDesktopServices>
#include <QCryptographicHash>

#include <QDebug>

class LiveObject::PrivateData {
public:
	QTcpSocket *socket;
	QTimer timer;
	bool registered;
	QUrl registerUrl;
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
	if (d->mac == "" || d->socket->state() != QAbstractSocket::ConnectedState || !d->registerUrl.isValid()) {
		return;
	}
	QDesktopServices::openUrl(d->registerUrl);
}

void LiveObject::connectToServer() {
	d->socket->abort();
	d->socket->connectToHost(TELLDUS_LIVE_HOST, TELLDUS_LIVE_PORT);
}

void LiveObject::disconnect() {
	d->socket->disconnectFromHost();
}

void LiveObject::readyRead() {
	QByteArray ba = d->socket->readAll();
	LiveMessage envelope = LiveMessage::fromByteArray(ba);
	QString signature = envelope.name();
	LiveMessage msg = LiveMessage::fromByteArray(envelope.argument(0));

	if (signatureForMessage(envelope.argument(0)) != signature) {
		qDebug() << "HASH mismatch!" << msg.name();
		return;
	}

	if (msg.name() == "") {
		return;
	} else if (msg.name() == "registered") {
		d->registered = true;
		emit registered();
	} else if (msg.name() == "notregistered") {
		d->registerUrl = msg.argument(0);
		emit notRegistered();
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
	//Create a new signed message
	QByteArray body = message.toByteArray();
	LiveMessage msg(signatureForMessage(body));
	msg.append(body);
	
	d->socket->write(msg.toByteArray());
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
	msg.append(TELLDUS_LIVE_PUBLIC_KEY);
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

QByteArray LiveObject::signatureForMessage( const QByteArray &message ) {
	QCryptographicHash signature( QCryptographicHash::Sha1 );
	signature.addData(message);
	signature.addData(TELLDUS_LIVE_PRIVATE_KEY);

	return signature.result().toHex();
}
