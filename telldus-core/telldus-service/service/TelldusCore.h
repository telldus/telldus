#ifndef TELLDUS_CORE_H
#define TELLDUS_CORE_H

#include <QObject>
#include <libtelldus-core/telldus-core-service.h>

class TelldusCorePrivate;

class TelldusCore :	public QObject {
	Q_OBJECT
public:
	TelldusCore(void);
	~TelldusCore(void);

	static void logMessage( const QString &message);
	static void logMessage( int message);
	static void logWinError( int errorNo);

private slots:
	void newConnection();
	void newEventConnection();
	void disconnected();
	void deviceEventSlot(int, int, const char *);
	void deviceInserted(int, int, const QString &);
	void deviceRemoved(int, int, const QString &);
	void managerDone();

signals:
	void deviceEventSignal(int, int, const char *);

private:
	TelldusCorePrivate * const d;

	static void WINAPI deviceEvent(int deviceId, int, const char *, int, void *);
	static int callbackId;
};

#endif