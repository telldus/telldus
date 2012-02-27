#include "controllersplugin.h"
#include "controllerlist.h"
#include "controller.h"
#include <QScriptEngine>
#include <QtDeclarative>


ControllersPlugin::ControllersPlugin ( QObject * parent )
		:QScriptExtensionPlugin( parent )
{
}

ControllersPlugin::~ControllersPlugin() {
}

void ControllersPlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.controllers") {
		qmlRegisterType<Controller>("Telldus", 1, 0, "Controller");

		QScriptValue qml = engine->globalObject().property("com").property("telldus").property("controllers");
		QScriptValue list = engine->newQObject(new ControllerList(), QScriptEngine::ScriptOwnership);
		qml.setProperty("list", list);
	}
}

QStringList ControllersPlugin::keys () const {
	return QStringList() << "com.telldus.controllers";
}

Q_EXPORT_PLUGIN2(ControllersInterface, ControllersPlugin)
