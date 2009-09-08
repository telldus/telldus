#include <QtCore>
#include <QLabel>
#include <QApplication>
#include <QScriptEngine>
#include "devicesplugin.h"

#include "../../TelldusGui/telldusgui.h"

DevicesPlugin::DevicesPlugin( QObject * parent )
	:QScriptExtensionPlugin( parent )
{
}

DevicesPlugin::~DevicesPlugin() {
}

void DevicesPlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.gui") {
		qDebug() << "TelldusGui initiating";
		
		QWidget *deviceWidget = tdDeviceWidget(0);
		connect(deviceWidget, SIGNAL(showMessage(const QString &, const QString &, const QString &)), qApp, SIGNAL(showMessage(const QString &, const QString &, const QString &)));
		connect(deviceWidget, SIGNAL(eventTriggered(const QString &, const QString &)), qApp, SLOT(eventTriggered(const QString &, const QString &)));

		QScriptValue value = engine->newQObject(deviceWidget);
		value.setProperty("TELLSTICK_DEVICE_ADDED", TELLSTICK_DEVICE_ADDED);
		value.setProperty("TELLSTICK_DEVICE_CHANGED", TELLSTICK_DEVICE_CHANGED);
		value.setProperty("TELLSTICK_DEVICE_REMOVED", TELLSTICK_DEVICE_REMOVED);
		value.setProperty("TELLSTICK_DEVICE_STATE_CHANGED", TELLSTICK_DEVICE_STATE_CHANGED);
		engine->globalObject().property("com").property("telldus").setProperty("gui", value);
	}
}

QStringList DevicesPlugin::keys () const {
	return QStringList() << "com.telldus.gui";
}

Q_EXPORT_PLUGIN2(DevicesInterface, DevicesPlugin)


