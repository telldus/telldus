#include "MessageReceiver.h"
#include "TelldusCore.h"

#include <QRegExp>

#include <Dbt.h>

class MessageReceiverPrivate {
public:
	WNDCLASS wndclass;
	HWND hWnd;
	HDEVNOTIFY deviceNotificationHandle;
};

MessageReceiver *MessageReceiver::instance = 0;

static const GUID GUID_DEVINTERFACE_USBRAW =
{ 0xA5DCBF10L, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };

MessageReceiver::MessageReceiver(QObject *parent)
	:QObject(parent)
{
	d = new MessageReceiverPrivate;
	TelldusCore::logMessage("MessageReceiver");

	MessageReceiver::instance = this;

	d->wndclass.cbClsExtra = 0;
	d->wndclass.cbWndExtra = 0;
	d->wndclass.hbrBackground = 0;
	d->wndclass.hCursor = NULL;
	d->wndclass.hIcon = NULL;
	d->wndclass.style = 0;
	d->wndclass.lpfnWndProc = reinterpret_cast<WNDPROC>(MyWindowProc);
	d->wndclass.hInstance = GetModuleHandle(NULL);
	d->wndclass.lpszMenuName = NULL;
	d->wndclass.lpszClassName = L"TelldusServiceHiddenMessageReceiver";
	
	if (!RegisterClassW(&d->wndclass)) {
		DWORD dw = GetLastError();
		TelldusCore::logWinError(dw);
		return;
	} else {
		TelldusCore::logMessage("Success");
	}

	d->hWnd = CreateWindowEx(
		0, d->wndclass.lpszClassName,
		L"TelldusService Message Receiver",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0, 0, d->wndclass.hInstance, NULL);

	if (!d->hWnd) {
		TelldusCore::logMessage("Failed window");
		return;
	} else {
		TelldusCore::logMessage("Success window");
	}

	DEV_BROADCAST_DEVICEINTERFACE devInterface;
	ZeroMemory( &devInterface, sizeof(devInterface) );
	devInterface.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	devInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	devInterface.dbcc_classguid = GUID_DEVINTERFACE_USBRAW;

	d->deviceNotificationHandle = RegisterDeviceNotification(d->hWnd, &devInterface, 0);

	TelldusCore::logMessage("Done");
}

MessageReceiver::~MessageReceiver(void) {
	UnregisterDeviceNotification(d->deviceNotificationHandle);
	delete d;
}

LRESULT MessageReceiver::MyWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if ( msg != WM_DEVICECHANGE) {
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	if (wParam != DBT_DEVICEARRIVAL && wParam != DBT_DEVICEREMOVECOMPLETE) {
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	PDEV_BROADCAST_DEVICEINTERFACE pDevInf = reinterpret_cast<PDEV_BROADCAST_DEVICEINTERFACE>(lParam);
	if (!pDevInf) {
		TelldusCore::logMessage("Could not cast");
		return 0;
	}

	QRegExp rx("USB#Vid_([0-9A-Fa-f]+)&Pid_([0-9A-Fa-f]+)#(\\w+)#");
	if (rx.indexIn(QString::fromWCharArray(pDevInf->dbcc_name)) < 0) {
		return 0;
	}

	int vid = strtol(rx.cap(1).toLocal8Bit(), 0, 16);
	int pid = strtol(rx.cap(2).toLocal8Bit(), 0, 16);

	if (wParam == DBT_DEVICEARRIVAL) {
		emit MessageReceiver::instance->deviceInserted(vid, pid, rx.cap(3));
	} else {
		emit MessageReceiver::instance->deviceRemoved(vid, pid, rx.cap(3));
	}
	return 0;
}
