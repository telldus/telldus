#ifndef DEVICE_H
#define DEVICE_H

#include <telldus-core.h>

#include <QObject>

class Device : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(Device)

public:
	Device(int id, int methodsSupported, QObject *parent = 0);

	~Device();

	int id() const;

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

	static void remove(int deviceId);

public slots:
	void remove();
	void save();
	void turnOff();
	void turnOn();
	void bell();
	void dim(unsigned char level);
	void learn();

signals:
	void deviceChanged( int deviceId, int eventId, int changeType );
	void deviceRemoved( int deviceId );
	void nameChanged( int deviceId, const QString &newName );
	void methodsChanged( int deviceId );
	void stateChanged( int deviceId );
// 	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );
// 	void eventTriggered( const QString &name, const QString &title );
	
private slots:
	void deviceChangedSlot(int deviceId, int eventId, int changeType);

private:
	static void WINAPI deviceEvent(int deviceId, int, const char *, int, void *);
	static void WINAPI deviceChangeEvent(int deviceId, int, int, int, void *);
	void triggerEvent( int message );

	class DevicePrivate;
	DevicePrivate *d;
	static int callbackId, deviceChangeCallbackId;
};

#endif // DEVICE_H
