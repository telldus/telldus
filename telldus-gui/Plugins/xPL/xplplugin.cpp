#include "xplplugin.h"
#include <QScriptEngine>
#include <QDebug>

#include <xplinstance.h>

QScriptValue xPLMessageToScriptValue(QScriptEngine *engine, xPLMessage* const &in) {
	return engine->newQObject(in);
}

void xPLMessageFromScriptValue(const QScriptValue &object, xPLMessage* &out) {
	out = qobject_cast<xPLMessage*>(object.toQObject());
}

QScriptValue xPLInstanceCtor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() < 2) {
		return context->throwError("xPLInstance constructor takes two parameters");
	}
	if (context->isCalledAsConstructor()) {
		xPLInstance *instance = new xPLInstance(context->argument(0).toString(), context->argument(1).toString());
		QScriptValue instanceValue = engine->newQObject(instance);
		context->setThisObject( instanceValue );
	}
	return engine->undefinedValue();
}

QScriptValue xPLMessageCtor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->isCalledAsConstructor()) {
		xPLMessage *msg;
		if (context->argumentCount() > 0) {
			int intMsi = context->argument(0).toInteger();
			xPLMessage::Identifier i = xPLMessage::xplcmnd;
			if (intMsi == xPLMessage::xplstat) {
				i = xPLMessage::xplstat;
			} else if (intMsi == xPLMessage::xpltrig) {
				i = xPLMessage::xpltrig;
			}
			msg = new xPLMessage(i);
		} else {
			msg = new xPLMessage();
		}
		QScriptValue msgValue = engine->newQObject(msg, QScriptEngine::ScriptOwnership);
		context->setThisObject( msgValue );
	}
	return engine->undefinedValue();
}


XPLPlugin::XPLPlugin ( QObject * parent )
		:QScriptExtensionPlugin( parent )
{
}

XPLPlugin::~XPLPlugin() {
}

void XPLPlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.xpl") {
		qScriptRegisterMetaType(engine, xPLMessageToScriptValue, xPLMessageFromScriptValue);

		engine->globalObject().setProperty("xPLInstance", engine->newFunction(xPLInstanceCtor));
		QScriptValue messageValue = engine->newFunction(xPLMessageCtor);
		messageValue.setProperty("xplcmnd", xPLMessage::xplcmnd);
		messageValue.setProperty("xplstat", xPLMessage::xplstat);
		messageValue.setProperty("xpltrig", xPLMessage::xpltrig);
		engine->globalObject().setProperty("xPLMessage", messageValue);
	}
}

QStringList XPLPlugin::keys () const {
	return QStringList() << "com.telldus.xpl";
}

Q_EXPORT_PLUGIN2(XPLInterface, XPLPlugin)
