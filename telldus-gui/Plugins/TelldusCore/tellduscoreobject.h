#ifndef TELLDUSCOREOBJECT_H
#define TELLDUSCOREOBJECT_H

#include <QObject>

class TelldusCoreObject : public QObject
{
	Q_OBJECT
public:
	TelldusCoreObject( QObject * parent = 0 );
	virtual ~TelldusCoreObject();
	
signals:
	void deviceChange( int deviceId, int eventId );

public slots:
	int getNumberOfDevices() const;
	int getDeviceId( int index ) const;
	QString getName( int deviceId ) const;
};

#endif // TELLDUSCOREOBJECT_H
