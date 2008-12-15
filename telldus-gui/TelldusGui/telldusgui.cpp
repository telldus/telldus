#include "telldusgui.h"

#include "devicewidget.h"

QWidget *tdDeviceWidget( QWidget *parent ) {
	return new DeviceWidget(parent);
}
