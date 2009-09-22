#include "telldusgui.h"
#include <QTranslator>
#include <QCoreApplication>
#include <QDebug>

#include "devicewidget.h"

QWidget* WINAPI tdDeviceWidget( QWidget *parent ) {
	QTranslator *translator = new QTranslator(parent);
	translator->load("TelldusGui_" + QLocale::system().name());
	qApp->installTranslator(translator);
	return new DeviceWidget(parent);
}
