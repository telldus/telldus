#ifndef TELLDUS_CORE_H
#define TELLDUS_CORE_H

#include <QObject>
#include <libtelldus-core/telldus-core.h>

class TelldusCorePrivate;

class TelldusCore :	public QObject {
	Q_OBJECT
public:
	TelldusCore(void);
	~TelldusCore(void);

	static void logMessage( const QString &message);

private slots:
	void newConnection();
	void newEventConnection();
	void disconnected();
	void deviceEventSlot(int, int, const char *);
	void managerDone();

signals:
	void deviceEventSignal(int, int, const char *);

private:
	TelldusCorePrivate * const d;

	static void WINAPI deviceEvent(int deviceId, int, const char *, int, void *);
	static int callbackId;
};

#endif