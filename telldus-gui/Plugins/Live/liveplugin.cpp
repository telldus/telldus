#include "liveplugin.h"
#include "livemessage.h"
#include "liveobject.h"
#include <QScriptEngine>
#include <QDebug>

QScriptValue LiveSocketCtor(QScriptContext *context, QScriptEngine *engine)
{
	/*if (context->argumentCount() < 2) {
		return context->throwError("LiveSocket constructor takes two parameters");
	}*/
	if (context->isCalledAsConstructor()) {
		LiveObject *object = new LiveObject(engine);
		QScriptValue instanceValue = engine->newQObject(object, QScriptEngine::ScriptOwnership);
		context->setThisObject( instanceValue );
	}
	return engine->undefinedValue();
}

QScriptValue LiveMessageToScriptValue(QScriptEngine *engine, LiveMessage* const &in) {
	return engine->newQObject(in);
}

void LiveMessageFromScriptValue(const QScriptValue &object, LiveMessage* &out) {
	out = qobject_cast<LiveMessage*>(object.toQObject());
}

QScriptValue LiveMessageCtor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() < 1) {
		return context->throwError("LiveSocket constructor takes two parameters");
	}
	if (context->isCalledAsConstructor()) {
		LiveMessage *msg = new LiveMessage(context->argument(0).toString());
		QScriptValue msgValue = engine->newQObject(msg, QScriptEngine::ScriptOwnership);
		context->setThisObject( msgValue );
	}
	return engine->undefinedValue();
}

LivePlugin::LivePlugin ( QObject * parent )
		:QScriptExtensionPlugin( parent )
{
}

LivePlugin::~LivePlugin() {
}

void LivePlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.live") {
		qScriptRegisterMetaType(engine, LiveMessageToScriptValue, LiveMessageFromScriptValue);

		engine->globalObject().setProperty("LiveSocket", engine->newFunction(LiveSocketCtor));
		engine->globalObject().setProperty("LiveMessage", engine->newFunction(LiveMessageCtor));
	}
}

QStringList LivePlugin::keys () const {
	return QStringList() << "com.telldus.live";
}

Q_EXPORT_PLUGIN2(LiveInterface, LivePlugin)
