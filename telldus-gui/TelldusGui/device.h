#ifndef DEVICE_H
#define DEVICE_H

#include <QHash>
#include <QPointer>

class Device : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(Device)

public:
	virtual ~Device();

	static Device *getDevice( int id );
	static Device *newDevice( );

public slots:
	void save();

signals:
	void deviceAdded( int id );

private:
	Device(int id);

	static QHash<int, Device *> devices;
	QHash<QString, QString> p_settings;
	int p_id;
};

#endif // DEVICE_H
