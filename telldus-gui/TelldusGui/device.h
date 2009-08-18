#ifndef DEVICE_H
#define DEVICE_H

#include <telldus-core.h>

#include <QHash>
#include <QPointer>
#include <QString>

class DevicePrivate;

class Device : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(Device)

public:
	~Device();

	static Device *getDevice( int id );
	static Device *newDevice( );
	static bool deviceLoaded( int id );

	int deviceId() const;

	void setModel( const QString &model );
	QString model() const;

	void setName( const QString & name );
	QString &name() const;

	void setParameter( const QString &name, const QString &value );
	QString parameter( const QString &name, const QString &defaultValue ) const;

	void setProtocol( const QString & protocol );
	QString &protocol() const;

	int deviceType() const;
	int lastSentCommand() const;
	QString lastSentValue() const;

	int methods() const;

public slots:
	void save();
	void turnOff();
	void turnOn();
	void bell();

signals:
	void deviceAdded( int id );
	void methodsChanged( int newMethods );
	void stateChanged( int deviceId, int newState );
	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );
	void eventTriggered( const QString &name, const QString &title );

private:
	Device(int id);
	void updateMethods();
	void updateState();
	void triggerEvent( int message );

	DevicePrivate *d;
	static QHash<int, Device *> devices;
	static void WINAPI deviceEvent(int deviceId, int, const char *, int, void *);

	static int callbackId;
};

#endif // DEVICE_H
