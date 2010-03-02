#ifndef TELLDUSSERVICE_WIN_H
#define TELLDUSSERVICE_WIN_H

#include <QObject>
#include <windows.h>

extern int g_argc;
extern char **g_argv;

class TelldusCore;

#define serviceName TEXT("TelldusCore")

class TelldusService : public QObject {
	Q_OBJECT
public:
    TelldusService();
    ~TelldusService();

	static void WINAPI serviceMain( DWORD /*argc*/, TCHAR* /*argv*/[] );

signals:
	void deviceInserted(int vid, int pid, const QString &serial);
	void deviceRemoved(int vid, int pid, const QString &serial);

protected:

    void start();
    void stop();

	DWORD WINAPI serviceControlHandler( DWORD controlCode, DWORD dwEventType, LPVOID lpEventData );

private:
	TelldusCore *tc;
	SERVICE_STATUS serviceStatus;
	SERVICE_STATUS_HANDLE serviceStatusHandle;

	static DWORD WINAPI serviceControlHandler( DWORD controlCode, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext );

};
#endif TELLDUSSERVICE_WIN_H