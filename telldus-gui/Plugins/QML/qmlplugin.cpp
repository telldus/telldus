#include "qmlplugin.h"
#include "qmlarray.h"
#include "qmlview.h"
#include <QScriptEngine>
#include <QScriptContextInfo>
#include <QFileInfo>
#include <QDir>

#include <QDebug>

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

QScriptValue QMLArrayFunction(QScriptContext *context, QScriptEngine *engine) {
	if (!context->isCalledAsConstructor()) {
		return engine->undefinedValue();
	}
	return engine->newQObject(new QMLArray(), QScriptEngine::ScriptOwnership);
}

void QMLPlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.qml") {
		QScriptValue qml = engine->globalObject().property("com").property("telldus").property("qml");
		qml.setProperty("view", engine->newFunction(QMLLoadFunction));
		qml.setProperty("array", engine->newFunction(QMLArrayFunction));
	}
}

QStringList QMLPlugin::keys () const {
	return QStringList() << "com.telldus.qml";
}

Q_EXPORT_PLUGIN2(QMLInterface, QMLPlugin)
