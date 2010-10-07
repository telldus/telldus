#include "TelldusWinService_win.h"
#include "TelldusMain.h"
//#include "TelldusCore.h"
//#include <QCoreApplication>
//#include <QRegExp>

#include <Dbt.h>

int g_argc;
char **g_argv;


static const GUID GUID_DEVINTERFACE_USBRAW =
{ 0xA5DCBF10L, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };

TelldusWinService::TelldusWinService()
	:tm(0)
{
	tm = new TelldusMain();

	//setServiceDescription("A Telldus service for managing TellStick Duo.");
}

TelldusWinService::~TelldusWinService() {
}

void TelldusWinService::start() {
//	tc = new TelldusCore();

//	connect(this, SIGNAL(deviceInserted(int,int,const QString &)), tc, SLOT(deviceInserted(int,int,const QString &)));
//	connect(this, SIGNAL(deviceRemoved(int,int,const QString &)), tc, SLOT(deviceRemoved(int,int,const QString &)));

	//app->quit();

}


void TelldusWinService::stop() {
	//running = false;
	tm->stop();
	//if(tm){
	//	delete tm;
	//}
	//tm = 0;
//	if (tc) {
//		delete tc;
//	}
//	tc = 0;
}

DWORD WINAPI TelldusWinService::serviceControlHandler( DWORD controlCode, DWORD dwEventType, LPVOID lpEventData ) {
	switch ( controlCode ) {
		case SERVICE_CONTROL_INTERROGATE:
			SetServiceStatus( serviceStatusHandle, &serviceStatus );
			return NO_ERROR;

		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			stop();
			serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus( serviceStatusHandle, &serviceStatus );

//			QCoreApplication::quit();
			return NO_ERROR;

		case SERVICE_CONTROL_DEVICEEVENT:
			if (dwEventType != DBT_DEVICEARRIVAL && dwEventType != DBT_DEVICEREMOVECOMPLETE) {
				return ERROR_CALL_NOT_IMPLEMENTED;
			}
			
			PDEV_BROADCAST_DEVICEINTERFACE pDevInf = reinterpret_cast<PDEV_BROADCAST_DEVICEINTERFACE>(lpEventData);
			if (!pDevInf) {
				return ERROR_CALL_NOT_IMPLEMENTED;
			}

//			TelldusCore::logMessage(QString::fromWCharArray(pDevInf->dbcc_name));

/*			QRegExp rx("USB#VID_([0-9A-Fa-f]+)&PID_([0-9A-Fa-f]+)", Qt::CaseInsensitive);
			if (rx.indexIn(QString::fromWCharArray(pDevInf->dbcc_name)) < 0) {
				TelldusCore::logMessage("No match");
				return 0;
			}
			
			int vid = strtol(rx.cap(1).toLocal8Bit(), 0, 16);
			int pid = strtol(rx.cap(2).toLocal8Bit(), 0, 16);
			
			if (dwEventType == DBT_DEVICEARRIVAL) {
				TelldusCore::logMessage(QString("Insert %1 %2").arg(vid).arg(pid));
				emit deviceInserted(vid, pid, "");
			} else {
				TelldusCore::logMessage(QString("Remove %1").arg(rx.cap(3)));
				emit deviceRemoved(vid, pid, "");
			}*/

			return NO_ERROR;
	}
	return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI TelldusWinService::serviceControlHandler( DWORD controlCode, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext ) {
	TelldusWinService *instance = reinterpret_cast<TelldusWinService *>(lpContext);
	if (!instance) {
		return ERROR_CALL_NOT_IMPLEMENTED;
	}
	return instance->serviceControlHandler(controlCode, dwEventType, lpEventData);
}

void WINAPI TelldusWinService::serviceMain( DWORD argc, TCHAR* argv[] ) {
	TelldusWinService instance;
//	QCoreApplication app(g_argc, g_argv);


	// initialise service status
	instance.serviceStatus.dwServiceType = SERVICE_WIN32;
	instance.serviceStatus.dwCurrentState = SERVICE_STOPPED;
	instance.serviceStatus.dwControlsAccepted = 0;
	instance.serviceStatus.dwWin32ExitCode = NO_ERROR;
	instance.serviceStatus.dwServiceSpecificExitCode = NO_ERROR;
	instance.serviceStatus.dwCheckPoint = 0;
	instance.serviceStatus.dwWaitHint = 0;

	instance.serviceStatusHandle = RegisterServiceCtrlHandlerEx( serviceName, TelldusWinService::serviceControlHandler, &instance );

	if ( instance.serviceStatusHandle ) {
		// service is starting
		instance.serviceStatus.dwCurrentState = SERVICE_START_PENDING;
		SetServiceStatus( instance.serviceStatusHandle, &instance.serviceStatus );

		// running
		instance.start();
		instance.serviceStatus.dwControlsAccepted |= (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		instance.serviceStatus.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus( instance.serviceStatusHandle, &instance.serviceStatus );

		DEV_BROADCAST_DEVICEINTERFACE devInterface;
		ZeroMemory( &devInterface, sizeof(devInterface) );
		devInterface.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
		devInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		devInterface.dbcc_classguid = GUID_DEVINTERFACE_USBRAW;

		HDEVNOTIFY deviceNotificationHandle = RegisterDeviceNotificationW(instance.serviceStatusHandle, &devInterface, DEVICE_NOTIFY_SERVICE_HANDLE);

		if (!deviceNotificationHandle) {
//			TelldusCore::logMessage(QString("Fail RegisterDeviceNotification"));
		}

		//TelldusCore::logMessage(QString("Main thread waiting for service to stop"));
		
		//TelldusMain tm;
		//tm = new TelldusMain();
		//tm.start();
		instance.tm->start();
		
		//while(instance.running) {
		//	Sleep(1000);
		//}


//		app.exec();
		//TelldusCore::logMessage(QString("Main thread waited, shutting down"));

		// service was stopped
		instance.serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus( instance.serviceStatusHandle, &instance.serviceStatus );

		// service is now stopped
		instance.serviceStatus.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		instance.serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus( instance.serviceStatusHandle, &instance.serviceStatus );
	}
}
