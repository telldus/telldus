#ifndef DEVICE_H
#define DEVICE_H

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

	void setModel( int model );
	int model();

	void setName( const QString & name );
	const QString &name();

	void setProtocol( const QString & protocol );
	const QString &protocol();

public slots:
	void save();

signals:
	void deviceAdded( int id );

private:
	Device(int id);

	static QHash<int, Device *> devices;
	QHash<QString, QString> p_settings;
	int p_id, p_model;
	QString p_name, p_protocol;
	bool p_modelChanged, p_nameChanged, p_protocolChanged;
};

#endif // DEVICE_H
