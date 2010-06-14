#ifndef MANAGER_H
#define MANAGER_H

#include <QThread>
#include <QByteArray>
#include <QVariant>
#include "Message.h"
#include <libtelldus-core/telldus-core.h>

class ManagerPrivate;

struct CallbackStruct {
	TDDeviceEvent event;
	int id;
	void *context;
};
struct DeviceChangeCallbackStruct {
	TDDeviceChangeEvent event;
	int id;
	void *context;
};
struct RawCallbackStruct {
	TDRawDeviceEvent event;
	int id;
	void *context;
};


class Manager : public QThread {
	Q_OBJECT
public:
	~Manager(void);

	static Manager *getInstance();
	static void close();

	QVariant send(const TelldusService::Message &msg, bool *success);

	int numberOfDevices();
	QString deviceName(int deviceId);

	int registerDeviceEvent( TDDeviceEvent eventFunction, void *context );
	int registerDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context );
	int registerRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context );
	bool unregisterCallback( int callbackId );

	static void logMessage( const QString &message);

protected:
	void run();

private slots:
	void dataReceived(const QByteArray &msg);

private:
	Manager(void);

	ManagerPrivate *d;
	static Manager *instance;
};

#endif //MANAGER_H
