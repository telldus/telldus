#ifndef DEVICE_H
#define DEVICE_H

#include "telldus-core.h"

#include <QHash>
#include <QPointer>
#include <QString>

class Device : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(Device)

public:
	~Device();

	static Device *getDevice( int id );
	static Device *newDevice( );
	static bool deviceLoaded( int id );

	void setModel( int model );
	int model();

	void setName( const QString & name );
	const QString &name();

	void setParameter( const QString &name, const QString &value );
	QString parameter( const QString &name, const QString &defaultValue ) const;

	void setProtocol( const QString & protocol );
	const QString &protocol();

	int deviceType() const;
	int lastSentCommand() const;

	int methods() const;

public slots:
	void save();
	void turnOff();
	void turnOn();
	void bell();

signals:
	void deviceAdded( int id );
	void methodsChanged( int newMethods );
	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );
	void eventTriggered( const QString &name, const QString &title );

private:
	Device(int id);
	void updateMethods();
	void triggerEvent( int message );

	static QHash<int, Device *> devices;
	QHash<QString, QString> p_settings;
	int p_id, p_model;
	QString p_name, p_protocol;
	bool p_modelChanged, p_nameChanged, p_protocolChanged;
	mutable int p_methods;
};

#endif // DEVICE_H
