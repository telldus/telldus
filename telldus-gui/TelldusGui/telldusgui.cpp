#include "telldusgui.h"

#include "devicewidget.h"

QWidget* WINAPI tdDeviceWidget( QWidget *parent ) {
	return new DeviceWidget(parent);
}
