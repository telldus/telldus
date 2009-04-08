#include <QtCore>
#include <QLabel>
#include <QApplication>
#include "devicesplugin.h"

#include "../../TelldusGui/telldusgui.h"

QIcon DevicesPlugin::iconForPage( const QString &page ) const {
	return QIcon(":/images/devices.png");
}

QString DevicesPlugin::pluginName() const {
	return "Devices";
}

QWidget *DevicesPlugin::widget( const QString &page, QWidget *parent ) const {
	QWidget *deviceWidget = tdDeviceWidget(parent);
	connect(deviceWidget, SIGNAL(showMessage(const QString &, const QString &, const QString &)), qApp, SIGNAL(showMessage(const QString &, const QString &, const QString &)));
	connect(deviceWidget, SIGNAL(eventTriggered(const QString &, const QString &)), qApp, SLOT(eventTriggered(const QString &, const QString &)));
	return deviceWidget;
}

QStringList DevicesPlugin::widgets() const {
	return QStringList() << "devices.default";
}

Q_EXPORT_PLUGIN2(DevicesInterface, DevicesPlugin)
