#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QByteArray>
#include <QVariant>
#include "Message.h"

class ManagerPrivate;

class Manager : public QObject {
	Q_OBJECT
public:
	~Manager(void);

	static Manager *getInstance();
	static void close();

	QVariant send(const Message &, const QVariant &);

	int numberOfDevices();
	QString deviceName(int deviceId);

private slots:
	void dataReceived();

private:
	Manager(void);

	ManagerPrivate *d;
	static Manager *instance;
};

#endif MANAGER_H