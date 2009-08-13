#include <QtCore>
#include <QLabel>
#include <QApplication>
#include <QScriptEngine>
#include "devicesplugin.h"

#include "../../TelldusGui/telldusgui.h"
#include "../../TelldusCenter/tellduscenterapplication.h"

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
		engine->globalObject().property("com").property("telldus").setProperty("gui", value);
	}
}

QStringList DevicesPlugin::keys () const {
	return QStringList() << "com.telldus.gui";
}

Q_EXPORT_PLUGIN2(DevicesInterface, DevicesPlugin)


