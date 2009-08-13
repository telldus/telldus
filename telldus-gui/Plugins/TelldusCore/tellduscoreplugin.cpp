#include "tellduscoreplugin.h"
#include "tellduscoreobject.h"
#include <QScriptEngine>
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
		engine->globalObject().property("com").property("telldus").setProperty("core", value);
	}
}

QStringList TelldusCorePlugin::keys () const {
	return QStringList() << "com.telldus.core";
}

Q_EXPORT_PLUGIN2(TelldusCoreInterface, TelldusCorePlugin)
