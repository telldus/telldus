#ifndef XPLDEVICE_H
#define XPLDEVICE_H

#include "xplmessage.h"
#include <QDateTime>
#include <QHostAddress>

class xPLDevicePrivate;

class QTXPL_EXPORT xPLDevice
{
public:
	xPLDevice( const xPLDevice & ); //Copy constructor
	xPLDevice& operator=( xPLDevice ); //Copy assignment operator

	xPLDevice( const QString &deviceName );
	xPLDevice( const QString &vendor, const QString &device );
	xPLDevice( const QString &vendor, const QString &device, const QString &instance );

	~xPLDevice();

	QString deviceName() const;

	QDateTime lastHeartBeat() const;

	QHostAddress address() const;
	int heartBeatInterval() const;
	int port() const;
	void setAddress( const QHostAddress &address );
	void setHeartBeatInterval( int minutes );
	void setPort( int port );
	void setLastHeartBeat( const QDateTime &timestamp );

private:
	xPLDevicePrivate *d;
};

#endif // XPLDEVICE_H
