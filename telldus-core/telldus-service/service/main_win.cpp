#include "TelldusCore.h"
#include <QCoreApplication>
#include <QFile>

#include "Windows.h"
#include <Dbt.h>

TCHAR* serviceName = TEXT("TelldusCore");

static const GUID GUID_DEVINTERFACE_USBRAW =
{ 0xA5DCBF10L, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };


class TelldusService {
public:
    TelldusService();
    ~TelldusService();

	static void WINAPI serviceMain( DWORD /*argc*/, TCHAR* /*argv*/[] );

protected:

    void start();
    void stop();

private:
	TelldusCore *tc;
	SERVICE_STATUS serviceStatus;
	HANDLE stopServiceEvent;
	SERVICE_STATUS_HANDLE serviceStatusHandle;

	static DWORD WINAPI serviceControlHandler( DWORD controlCode, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext );

};

TelldusService::TelldusService()
	:tc(0)
{
	
    //setServiceDescription("A Telldus service for managing TellStick Duo.");
}

TelldusService::~TelldusService()
{
}

void TelldusService::start() {

	QFile file("C:/log.txt");
	file.open(QIODevice::Text | QIODevice::WriteOnly);
	file.close();

	tc = new TelldusCore();

	//app->quit();

}

void TelldusService::stop() {
	if (tc) {
		delete tc;
	}
	tc = 0;
}

DWORD WINAPI TelldusService::serviceControlHandler( DWORD controlCode, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext ) {
	TelldusService *instance = reinterpret_cast<TelldusService *>(lpContext);
	if (!instance) {
		return ERROR_CALL_NOT_IMPLEMENTED;
	}
	switch ( controlCode ) {
		case SERVICE_CONTROL_INTERROGATE:
			SetServiceStatus( instance->serviceStatusHandle, &instance->serviceStatus );
			return NO_ERROR;

		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			instance->stop();
			instance->serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus( instance->serviceStatusHandle, &instance->serviceStatus );

			SetEvent( instance->stopServiceEvent );
			return NO_ERROR;

		case SERVICE_CONTROL_DEVICEEVENT:
			TelldusCore::logMessage("DeviceEvent wihoo");
			if (dwEventType == DBT_DEVICEARRIVAL || dwEventType == DBT_DEVICEREMOVECOMPLETE) {
				TelldusCore::logMessage("Device add/remove");
			
				PDEV_BROADCAST_DEVICEINTERFACE pDevInf = reinterpret_cast<PDEV_BROADCAST_DEVICEINTERFACE>(lpEventData);
				if (!pDevInf) {
					TelldusCore::logMessage("Could not cast");
					return NO_ERROR;
				}

				TelldusCore::logMessage(QString::fromWCharArray(pDevInf->dbcc_name));
			}

			return NO_ERROR;
	}

	return ERROR_CALL_NOT_IMPLEMENTED;
}

void WINAPI TelldusService::serviceMain( DWORD /*argc*/, TCHAR* /*argv*/[] ) {
	TelldusService instance;

	// initialise service status
	instance.serviceStatus.dwServiceType = SERVICE_WIN32;
	instance.serviceStatus.dwCurrentState = SERVICE_STOPPED;
	instance.serviceStatus.dwControlsAccepted = 0;
	instance.serviceStatus.dwWin32ExitCode = NO_ERROR;
	instance.serviceStatus.dwServiceSpecificExitCode = NO_ERROR;
	instance.serviceStatus.dwCheckPoint = 0;
	instance.serviceStatus.dwWaitHint = 0;

	instance.serviceStatusHandle = RegisterServiceCtrlHandlerEx( serviceName, TelldusService::serviceControlHandler, &instance );

	if ( instance.serviceStatusHandle ) {
		// service is starting
		instance.serviceStatus.dwCurrentState = SERVICE_START_PENDING;
		SetServiceStatus( instance.serviceStatusHandle, &instance.serviceStatus );

		// do initialisation here
		instance.stopServiceEvent = CreateEvent( 0, FALSE, FALSE, 0 );

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
			TelldusCore::logMessage(QString("Fail RegisterDeviceNotification"));
		} else {
			TelldusCore::logMessage(QString("Success RegisterDeviceNotification"));
		}


		TelldusCore::logMessage(QString("Main thread waiting for service to stop"));
		WaitForSingleObject( instance.stopServiceEvent, INFINITE );
		TelldusCore::logMessage(QString("Main thread waited, shutting down"));

		// service was stopped
		instance.serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus( instance.serviceStatusHandle, &instance.serviceStatus );

		// do cleanup here
		CloseHandle( instance.stopServiceEvent );
		instance.stopServiceEvent = 0;

		// service is now stopped
		instance.serviceStatus.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		instance.serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus( instance.serviceStatusHandle, &instance.serviceStatus );
	}
}

int main(int argc, char **argv) {

	SERVICE_TABLE_ENTRY serviceTable[] = {
		{serviceName, TelldusService::serviceMain },
        { 0, 0 }
    };

	QCoreApplication app(argc, argv);

    StartServiceCtrlDispatcher( serviceTable );

	return 0;
}
