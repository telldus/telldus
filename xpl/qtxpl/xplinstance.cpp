#include "xplinstance.h"
#include "xplmessage.h"

#include <QUdpSocket>
#include <QLinkedList>
#include <QNetworkInterface>

#include <QDebug>
#include <QTimer>

class xPLInstancePrivate {
public:
	xPLDevice *thisDevice;
	QLinkedList<xPLDevice> devices;
	QUdpSocket *socket;
	xPLInstance::OperationMode mode;
	QHostAddress address;
	bool isAttatched;
	QTimer *timer;
};

xPLInstance::xPLInstance( const xPLDevice &device, QObject *parent )
		:QObject(parent)
{
	d = new xPLInstancePrivate;
	d->thisDevice = new xPLDevice(device);
	d->mode = Disconnected;
	d->timer = new QTimer(this);
	this->init();
}

xPLInstance::xPLInstance( const QString &vendor, const QString &device, QObject *parent )
		:QObject(parent)
{
	d = new xPLInstancePrivate;
	d->thisDevice = new xPLDevice(vendor, device);
	d->mode = Disconnected;
	d->timer = new QTimer(this);
	this->init();
}

xPLInstance::~xPLInstance() {
	delete d->thisDevice;
	delete d;
}

void xPLInstance::init() {
	foreach(QHostAddress address, QNetworkInterface::allAddresses()) {
		if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost) {
			d->address = address;
			break;
		}
	}
	d->isAttatched = false;
	d->socket = new QUdpSocket(this);
	d->thisDevice->setPort(this->bindToPort());
	if (d->thisDevice->port() == 0) {
		return;
	}
	d->timer->setInterval( 3000 );
	connect(d->socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
	connect(d->timer, SIGNAL(timeout()), this, SLOT(poll()));
	this->sendHeartbeat();
	d->timer->start();
}

bool xPLInstance::attatched() const {
	return this->d->isAttatched;
}

int xPLInstance::bindToPort() {
	//Hub-mode not supported
	/*if (d->socket->bind( XPL_PORT )) {
		d->mode = Hub;
		qDebug("Bind succeded as hub");
		return XPL_PORT;
	}*/
	for (int port = 49152; port <= 65535; ++port) {
		if (d->socket->bind( port, QUdpSocket::DontShareAddress )) {
			d->mode = Client;
			qDebug() << "Bind succeded as client on" << port;
			return port;
		}
	}
	qDebug("Bind failed");
	return 0;
}

xPLInstance::OperationMode xPLInstance::operationMode() const {
	return d->mode;
}

void xPLInstance::processPendingDatagrams() {
	while (d->socket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(d->socket->pendingDatagramSize());
		d->socket->readDatagram(datagram.data(), datagram.size());
		xPLMessage *message = xPLMessage::createMessageFromString( datagram.data() );
		if (message) {
			processMessage( *message );
			delete message;
		}
 }
}

void xPLInstance::poll() {
	if (!attatched()) {
		sendHeartbeat();
		return;
	}
	if (d->thisDevice->lastHeartBeat().secsTo( QDateTime::currentDateTime() ) >= d->thisDevice->heartBeatInterval() * 60) {
		sendHeartbeat();
	}
	//Loop all devices to see if they have timed out
	for( QLinkedList<xPLDevice>::iterator it = d->devices.begin(); it != d->devices.end(); ) {
		if ((*it).lastHeartBeat().secsTo( QDateTime::currentDateTime() ) >= (*it).heartBeatInterval() * 60 * 2) {
			qDebug() << "Device removed (timeout):" << (*it).deviceName();
			it = d->devices.erase( it );
			continue;
		}
		++it;
	}
}

void xPLInstance::sendMessage( const xPLMessage &msg ) const {
	xPLMessage message(msg); //Copy so we don't have const
	message.setSource(d->thisDevice->deviceName());
	QByteArray datagram = message.toString().toAscii();
	d->socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, XPL_PORT);
}

void xPLInstance::sendMessage( xPLMessage * message ) const {
	this->sendMessage(*message);
}

void xPLInstance::sendMessage( const xPLMessage &message, const xPLDevice &device ) const {
	QByteArray datagram = message.toString().toAscii();
	d->socket->writeDatagram(datagram.data(), datagram.size(), device.address(), device.port());
}

void xPLInstance::processHeartBeat( const xPLMessage &message ) {
	if (message.messageSchemeIdentifier() == "hbeat.request") {
		sendHeartbeat();
		return;
	}
	for( QLinkedList<xPLDevice>::iterator it = d->devices.begin(); it != d->devices.end(); ++it ) {
		if ( (*it).deviceName() == message.source() ) {
			if (message.messageSchemeIdentifier() == "hbeat.end") {
				qDebug() << "Device removed:" << message.source();
				d->devices.erase( it );
				return;
			} else {
				(*it).setLastHeartBeat( QDateTime::currentDateTime() );
				(*it).setHeartBeatInterval( message.bodyItem( "interval" ).toInt() );
			}
			return;
		}
	}
	if (!message.messageSchemeIdentifier().endsWith("app") && !message.messageSchemeIdentifier().endsWith("basic")) {
		return;
	}
	qDebug() << "New device:" << message.source();
	xPLDevice device( message.source() );
	device.setHeartBeatInterval( message.bodyItem("interval").toInt() );
	device.setLastHeartBeat( QDateTime::currentDateTime() );
	if (message.messageSchemeIdentifier() == "hbeat.app") {
		device.setAddress( QHostAddress(message.bodyItem("remote-ip")) );
		device.setPort( message.bodyItem("port").toInt() );
	}
	d->devices.append( device );
}

void xPLInstance::processMessage( const xPLMessage &message ) {
	if (!attatched() && message.messageSchemeIdentifier() == "hbeat.app" && message.source() == d->thisDevice->deviceName()) {
		//Our own echo
		setAttatched( true );
		return;
	}
	if (message.source() == d->thisDevice->deviceName()) {
		//Ignore messages from ourselves
		return;
	}
	if (message.messageSchemeIdentifier().startsWith("hbeat") || message.messageSchemeIdentifier().startsWith("config")) {
		processHeartBeat( message );
	}
	if (d->mode == xPLInstance::Hub) {
		//Reply the message to all clients
		for( QLinkedList<xPLDevice>::iterator it = d->devices.begin(); it != d->devices.end(); ++it ) {
			sendMessage(message, *it);
		}
	}
	if (message.target() != d->thisDevice->deviceName() && message.target() != "*") {
		//Message not for us
		return;
	}
	xPLMessage newMsg(message);
	emit messageReceived(&newMsg);
}

void xPLInstance::sendHeartbeat() {
	xPLMessage message( xPLMessage::xplstat );
	message.setMessageSchemeIdentifier( "hbeat.app" );
	message.setTarget("*");
	message.setSource( d->thisDevice->deviceName() );
	message.addBodyItem( "interval", QString::number(d->thisDevice->heartBeatInterval()) );
	message.addBodyItem( "port", QString::number(d->thisDevice->port()) );
	message.addBodyItem( "remote-ip", d->address.toString() );
	sendMessage( message );
	d->thisDevice->setLastHeartBeat( QDateTime::currentDateTime() );
}

void xPLInstance::setAttatched( bool attatched ) {
	d->isAttatched = attatched;
	if (d->isAttatched) {
		qDebug() << "Attached to network!";
		d->timer->setInterval( 60000 ); //Once a minute

		xPLMessage message( xPLMessage::xplcmnd );
		message.setTarget( "*" );
		message.setSource( d->thisDevice->deviceName() );
		message.setMessageSchemeIdentifier( "hbeat.request" );
		message.addBodyItem("command", "request");
		sendMessage( message );
		emit attachedToNetwork();
	}
}
