#include "xpldevice.h"

#include <QDebug>
#include <stdlib.h>
#include <time.h>

const int DEFAULT_HBEAT_INTERVAL = 5;

class xPLDevicePrivate {
public:
	QString vendor, device, instance;
	QDateTime lastHeartBeat;
	int port, heartBeatInterval;
	QHostAddress address;
};

xPLDevice::xPLDevice( const xPLDevice &other )
	: d( new xPLDevicePrivate( *other.d ))
{
	//do nothing
}

xPLDevice& xPLDevice::operator=( xPLDevice other ) {
	std::swap( this->d, other.d );
	return *this;
}

xPLDevice::xPLDevice( const QString &deviceName )
	:d(new xPLDevicePrivate)
{
	d->port = 0;
	d->heartBeatInterval = DEFAULT_HBEAT_INTERVAL;
	QString name = deviceName.section('.', 0, 0);
	d->vendor = name.section('-', 0, 0);
	d->device = name.section('-', 1, 1);
	d->instance = deviceName.section('.', 1);
}

xPLDevice::xPLDevice( const QString &vendor, const QString &device )
	:d(new xPLDevicePrivate)
{
	d->vendor = vendor;
	d->device = device;
	d->instance = "";
	d->port = 0;
	d->heartBeatInterval = DEFAULT_HBEAT_INTERVAL;

	//Seed the random number generator
	srand( (unsigned int)time( NULL ) );

	//Generate an instance
	int randomNumber = rand() % 9999 + 1; //Generate ranom number between 1-9999
	d->instance = QString("default%1").arg(randomNumber);
}

xPLDevice::xPLDevice( const QString &vendor, const QString &device, const QString &instance )
	:d(new xPLDevicePrivate)
{
	d->vendor = vendor;
	d->device = device;
	d->instance = instance;
	d->port = 0;
	d->heartBeatInterval = DEFAULT_HBEAT_INTERVAL;

}

xPLDevice::~xPLDevice() {
	delete d;
}

QHostAddress xPLDevice::address() const {
	return d->address;
}

int xPLDevice::heartBeatInterval() const {
	return d->heartBeatInterval;
}

QString xPLDevice::deviceName() const {
	return QString("%1-%2.%3").arg(d->vendor).arg(d->device).arg(d->instance);
}

QDateTime xPLDevice::lastHeartBeat() const {
	return d->lastHeartBeat;
}

int xPLDevice::port() const {
	return d->port;
}

void xPLDevice::setAddress( const QHostAddress &address ) {
	d->address = address;
}

void xPLDevice::setHeartBeatInterval( int minutes ) {
	d->heartBeatInterval = minutes;
}

void xPLDevice::setPort( int p ) {
	d->port = p;
}

void xPLDevice::setLastHeartBeat( const QDateTime &timestamp ) {
	d->lastHeartBeat = timestamp;
}
