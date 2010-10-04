#include "liveobject.h"
#include "config.h"
#include <QtNetwork>
#include <QtCore>
#include <QDesktopServices>
#include <QtCrypto>

#include <QDebug>

class LiveObject::PrivateData {
public:
	class Server;

	QSslSocket *socket;
	QTimer timer;
	bool registered;
	QUrl registerUrl;
	QString mac, hashMethod;
	QCA::Initializer qcaInit;
	QNetworkAccessManager *manager;
	QList<Server> serverList;
	QDateTime serverRefreshTime;
};

class LiveObject::PrivateData::Server {
public:
	QString address;
	int port;
};


LiveObject::LiveObject( QScriptEngine *engine, QObject * parent )
		: QObject(parent)
{
	d = new PrivateData;
	d->hashMethod = "sha1";
	foreach(QString hash, QStringList() << "sha512" << "sha256") {
		if (QCA::isSupported(hash.toLocal8Bit())) {
			d->hashMethod = hash;
			break;
		}
	}
	d->registered = false;
	d->socket = new QSslSocket(this);
	d->socket->setProtocol( QSsl::TlsV1 );
	connect(d->socket, SIGNAL(encrypted()), this, SLOT(p_connected()));
	connect(d->socket, SIGNAL(disconnected()), this, SLOT(p_disconnected()));
	connect(d->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(d->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	connect(d->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
	connect(d->socket, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(sslErrors(const QList<QSslError> &)));

	d->timer.setInterval(60000); //Once a minute
	connect(&d->timer, SIGNAL(timeout()), this, SLOT(pingServer()));

	d->manager = new QNetworkAccessManager(this);
	connect(d->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(serverAssignReply(QNetworkReply*)));
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
	if (d->serverList.isEmpty()) {
		qDebug() << "No servers to connect to, refresh list";
		d->manager->get(QNetworkRequest(QUrl(TELLDUS_LIVE_URI)));
	} else if (d->serverRefreshTime.secsTo(QDateTime::currentDateTime()) > 300) { //Valid 5 min
		qDebug() << "Serverlist to old, refresh";
		d->serverList.clear();
		d->manager->get(QNetworkRequest(QUrl(TELLDUS_LIVE_URI)));
	} else {
		d->socket->abort();
		PrivateData::Server server = d->serverList.takeFirst();
		qDebug() << "Connecting to" << server.address;
		d->socket->connectToHostEncrypted(server.address, server.port);
	}
}

void LiveObject::disconnect() {
	d->socket->disconnectFromHost();
}

void LiveObject::readyRead() {
	QByteArray ba = d->socket->readAll();
	qDebug() << ba;
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
	msg.append(d->hashMethod);
	this->sendMessage(msg);
	emit connected();
}

void LiveObject::p_disconnected() {
	d->timer.stop();
	d->registered = false;
}

void LiveObject::error( QAbstractSocket::SocketError socketError ) {
	qDebug() << "Error:" << socketError;
}

void LiveObject::stateChanged( QAbstractSocket::SocketState socketState ) {
	if (socketState == QAbstractSocket::UnconnectedState) {
		QTimer::singleShot(10000, this, SLOT(connectToServer()));
		qDebug() << "Reconnect in 10 seconds...";
	} else if (socketState == QAbstractSocket::ConnectingState) {
		qDebug() << "Connecting...";
	} else {
		//qDebug() << "State:" << socketState;
	}
}

void LiveObject::sslErrors( const QList<QSslError> & errors ) {
	bool everythingOK = true;
	foreach( QSslError error, errors ) {
		switch( error.error() ) {
			case QSslError::SelfSignedCertificate:
				continue;
			default:
				everythingOK = false;
				qDebug() << "SSLErrors" << error;
				break;
		}
	}
	if (everythingOK) {
		d->socket->ignoreSslErrors();
	}
}

void LiveObject::serverAssignReply( QNetworkReply *r ) {
	qDebug() << "Server assign reply";
	QXmlStreamReader xml(r);
	xml.readNextStartElement(); // enter <servers>

	bool found = false;
	while (xml.readNextStartElement()) {
		if (xml.name().toString().toUpper() == "SERVER") {
			QXmlStreamAttributes attrs = xml.attributes();
			PrivateData::Server server;
			server.address = attrs.value("address").toString();
			server.port = attrs.value("port").toString().toInt();
			d->serverList << server;
			found = true;
		} else {
			xml.skipCurrentElement();
		}

	}
	if (found) {
		qDebug() << "Servers found, retry direct...";
		d->serverRefreshTime = QDateTime::currentDateTime();
		QTimer::singleShot(0, this, SLOT(connectToServer()));
	} else {
		QTimer::singleShot(10000, this, SLOT(connectToServer()));
		qDebug() << "No server found, retry in 10 seconds...";
	}
	r->deleteLater();
}

QByteArray LiveObject::signatureForMessage( const QByteArray &message ) {
	if (QCA::isSupported(d->hashMethod.toLocal8Bit())) {
		QCA::Hash signature(d->hashMethod);
		signature.update(message);
		signature.update(TELLDUS_LIVE_PRIVATE_KEY);
		return signature.final().toByteArray().toHex();
	}
	
	//Fallback to builtin function
	QCryptographicHash signature( QCryptographicHash::Sha1 );
	signature.addData(message);
	signature.addData(TELLDUS_LIVE_PRIVATE_KEY);
	return signature.result().toHex();
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

