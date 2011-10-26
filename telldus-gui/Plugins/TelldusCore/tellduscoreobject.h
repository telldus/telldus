#ifndef TELLDUSCOREOBJECT_H
#define TELLDUSCOREOBJECT_H

#include <QObject>
#include <QVariantMap>
#include <telldus-core.h>

class TelldusCoreObject : public QObject
{
	Q_OBJECT
public:
	TelldusCoreObject( QObject * parent = 0 );
	virtual ~TelldusCoreObject();

signals:
	void deviceChange( int deviceId, int eventId );
	void deviceEvent( int deviceId, int method, const QString &data );
	void sensorEvent( const QString &protocol, const QString &model, int id, int dataType, const QString &value, int timestamp);
	void errorOccurred( int deviceId, int errorId, const QString &errorString );

public slots:
	int bell( int deviceId );
	int dim( int deviceId, char level );
	QString getName( int deviceId ) const;
	int getNumberOfDevices() const;
	int getDeviceId( int index ) const;
	int lastSentCommand( int deviceId, int methodsSupported );
	QString lastSentValue( int deviceId );
	int methods( int deviceId, int methodsSupported );

	QVariant sensor() const;
	QVariant sensorValue(const QString &protocol, const QString &model, int id, int dataType) const;

	int turnOn( int deviceId );
	int turnOff( int deviceId );
	int up( int deviceId );
	int down( int deviceId );
	int stop( int deviceId );

private:
	void triggerError( int, int );

	static void WINAPI deviceChangeEventCallback(int deviceId, int eventId, int changeType, int callbackId, void *context);
	static void WINAPI deviceEventCallback(int deviceId, int method, const char *data, int callbackId, void *context);
	static void WINAPI sensorEventCallback(const char *protocol, const char *model, int id, int dataType, const char *value, int timestamp, int callbackId, void *context);

	int deviceEventId;
	int deviceChangeEventId;
	int sensorEventId;

};

#endif // TELLDUSCOREOBJECT_H
