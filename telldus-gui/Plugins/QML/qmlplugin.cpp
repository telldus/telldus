#include "qmlplugin.h"
#include <QScriptEngine>
#include <QDeclarativeView>
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

QScriptValue QMLLoadFunction(QScriptContext *context, QScriptEngine *engine)
{
	qDebug() << "Tried to load qml-file";
	if (context->argumentCount() < 1) {
		return context->throwError("QML constructor takes one parameters");
	}

	QString filename = context->argument(0).toString();

	//Find out the filename so we have a relative directory to load from
	QScriptContextInfo info(context->parentContext());
	QFileInfo fileinfo(info.fileName());
	QDir dir = fileinfo.dir();
	qDebug() << dir.filePath(filename);

	QDeclarativeView *view = new QDeclarativeView();
	view->setSource(dir.filePath(filename));
	QScriptValue value = engine->newQObject(view, QScriptEngine::ScriptOwnership);
	return value;
}

void QMLPlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.qml") {
		qDebug() << "Hello from QML";

		engine->globalObject().property("com").property("telldus").property("qml").setProperty("load", engine->newFunction(QMLLoadFunction));
	}
}

QStringList QMLPlugin::keys () const {
	return QStringList() << "com.telldus.qml";
}

Q_EXPORT_PLUGIN2(QMLInterface, QMLPlugin)
