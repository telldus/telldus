#ifndef TELLDUSSERVICE_WIN_H
#define TELLDUSSERVICE_WIN_H

#include <windows.h>

extern int g_argc;
extern char **g_argv;

class TelldusMain;

#define serviceName TEXT("TelldusCore")

class TelldusWinService {
public:
	TelldusWinService();
	~TelldusWinService();

	static void WINAPI serviceMain( DWORD /*argc*/, TCHAR* /*argv*/[] );
	
protected:

	void stop();

	DWORD WINAPI serviceControlHandler( DWORD controlCode, DWORD dwEventType, LPVOID lpEventData );
	DWORD WINAPI deviceNotificationHandler( DWORD controlCode, DWORD dwEventType, LPVOID lpEventData );

private:
	TelldusMain *tm;
	SERVICE_STATUS serviceStatus;
	SERVICE_STATUS_HANDLE serviceStatusHandle;

	static DWORD WINAPI serviceControlHandler( DWORD controlCode, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext );

};
#endif TELLDUSSERVICE_WIN_H
