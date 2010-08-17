#ifndef TELLDUS_CORE_H
#define TELLDUS_CORE_H

#include <QObject>
#include "Pipe.h"
#include "Message.h"
#include <libtelldus-core/telldus-core-service.h>

class TelldusCorePrivate;
class Socket;

class TelldusCore :	public QObject {
	Q_OBJECT
public:
	TelldusCore(void);
	~TelldusCore(void);

	static void logMessage( const QString &message);
	static void logMessage( const std::string &message);
	static void logMessage( const char *message);
	static void logMessage( int message);
	static void logWinError( int errorNo);

public slots:
	void deviceInserted(int, int, const QString &);
	void deviceRemoved(int, int, const QString &);

private slots:
	void newConnection(Socket *);
	void newEventConnection(Socket *);
	void deviceEventSlot(int, int, const char *);
	void deviceChangeEventSlot(int, int, int);
	void rawDeviceEventSlot(const QString &, int);
	void managerDone();

signals:
	void deviceEventSignal(int, int, const char *);
	void deviceChangeEventSignal(int, int, int);
	void rawDeviceEventSignal(const QString &, int);

private:
	void disconnected(Socket *);
	void sendEventMessage(const TelldusService::Message &msg);
	TelldusCorePrivate * const d;

	static void WINAPI deviceEvent(int deviceId, int, const char *, int, void *);
	static void WINAPI deviceChangeEvent(int deviceId, int eventId, int changeType, int, void *);
	static void WINAPI rawDeviceEvent(const char *data, int controllerId, int, void *);
	static int callbackId;
};

#endif
