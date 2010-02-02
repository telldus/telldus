#include "telldusgui.h"
#include <QTranslator>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

#include "devicewidget.h"

QWidget* WINAPI tdDeviceWidget( QWidget *parent ) {
	QTranslator *translator = new QTranslator(parent);
#ifdef Q_OS_MAC
	QDir resDir = QDir(qApp->applicationDirPath());
        if (resDir.dirName() == "MacOS") {
                resDir.cdUp();
                resDir.cd("resources");
        }
        translator->load("TelldusGui_" + QLocale::system().name(), resDir.absolutePath());
#else
	translator->load("TelldusGui_" + QLocale::system().name());
#endif
	qApp->installTranslator(translator);
	return new DeviceWidget(parent);
}
