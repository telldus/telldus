#include "tellduscoreplugin.h"
#include "tellduscoreobject.h"
#include <QScriptEngine>
#include <telldus-core.h>
#include <QDebug>

TelldusCorePlugin::TelldusCorePlugin ( QObject * parent )
		:QScriptExtensionPlugin( parent )
{
}

TelldusCorePlugin::~TelldusCorePlugin() {
}

void TelldusCorePlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.core") {
		TelldusCoreObject *object = new TelldusCoreObject(engine);
		QScriptValue value = engine->newQObject(object);
		value.setProperty("TELLSTICK_TURNON", TELLSTICK_TURNON);
		value.setProperty("TELLSTICK_TURNOFF", TELLSTICK_TURNOFF);
		value.setProperty("TELLSTICK_BELL", TELLSTICK_BELL);
		value.setProperty("TELLSTICK_TOGGLE", TELLSTICK_TOGGLE);
		value.setProperty("TELLSTICK_DIM", TELLSTICK_DIM);
		engine->globalObject().property("com").property("telldus").setProperty("core", value);
	}
}

QStringList TelldusCorePlugin::keys () const {
	return QStringList() << "com.telldus.core";
}

Q_EXPORT_PLUGIN2(TelldusCoreInterface, TelldusCorePlugin)
