#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
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
struct RawCallbackStruct {
	TDRawDeviceEvent event;
	int id;
	void *context;
};


class Manager : public QObject {
	Q_OBJECT
public:
	~Manager(void);

	static Manager *getInstance();
	static void close();

	QVariant send(const Message &, const QVariant &);

	int numberOfDevices();
	QString deviceName(int deviceId);

	int registerDeviceEvent( TDDeviceEvent eventFunction, void *context );
	int registerRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context );

private slots:
	void dataReceived();

private:
	Manager(void);

	ManagerPrivate *d;
	static Manager *instance;
};

#endif MANAGER_H