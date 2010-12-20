#include "settingsplugin.h"
#include "settings.h"
#include <QScriptValue>
#include <QScriptContext>
#include <QScriptEngine>

#include <QDebug>

SettingsPlugin::SettingsPlugin ( QObject * parent )
		:QScriptExtensionPlugin( parent )
{
}

SettingsPlugin::~SettingsPlugin() {
}

QScriptValue SettingsConstructorFunction(QScriptContext *context, QScriptEngine *engine) {
	if (!context->isCalledAsConstructor()) {
		return engine->undefinedValue();
	}

	QString extension = engine->evaluate("__extension__").toString();
	return engine->newQObject(new Settings(extension), QScriptEngine::ScriptOwnership);
}

void SettingsPlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.settings") {
		engine->globalObject().property("com").property("telldus").setProperty("settings", engine->newFunction(SettingsConstructorFunction));
	}
}

QStringList SettingsPlugin::keys () const {
	return QStringList() << "com.telldus.settings";
}

Q_EXPORT_PLUGIN2(SettingsInterface, SettingsPlugin)
