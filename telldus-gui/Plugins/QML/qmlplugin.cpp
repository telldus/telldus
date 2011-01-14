#include "qmlplugin.h"
#include "qmlview.h"
#include <QScriptEngine>
#include <QScriptContextInfo>
#include <QFileInfo>
#include <QDir>

QMLPlugin::QMLPlugin ( QObject * parent )
		:QScriptExtensionPlugin( parent )
{
}

QMLPlugin::~QMLPlugin() {
}

QScriptValue QMLLoadFunction(QScriptContext *context, QScriptEngine *engine) {
	if (!context->isCalledAsConstructor()) {
		return engine->undefinedValue();
	}

	//Find out the filename so we have a relative directory to load from
	QScriptContextInfo info(context->parentContext());
	QFileInfo fileinfo(info.fileName());
	QDir dir = fileinfo.dir();

	return engine->newQObject(new QMLView(dir, context->argument(0)), QScriptEngine::ScriptOwnership);
}

void QMLPlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.qml") {
		engine->globalObject().property("com").property("telldus").property("qml").setProperty("view", engine->newFunction(QMLLoadFunction));
	}
}

QStringList QMLPlugin::keys () const {
	return QStringList() << "com.telldus.qml";
}

Q_EXPORT_PLUGIN2(QMLInterface, QMLPlugin)
