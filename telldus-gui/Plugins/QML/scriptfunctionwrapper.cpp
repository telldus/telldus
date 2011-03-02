#include "scriptfunctionwrapper.h"

#include <QScriptContext>
#include <QScriptEngine>
#include <QDebug>

class ScriptFunctionWrapper::PrivateData {
public:
	QScriptValue object;
	QString name;
};

ScriptFunctionWrapper::ScriptFunctionWrapper(const QScriptValue &object, const QString &name, QObject *parent) :
	QObject(parent)
{
	d = new PrivateData;
	d->object = object;
	d->name = name;
}

ScriptFunctionWrapper::~ScriptFunctionWrapper() {
	delete d;
}

QScriptValue ScriptFunctionWrapper::call() {
	return d->object.property(d->name).call();
}

QScriptValue ScriptFunctionWrapper::callWith(const QScriptValue &val) {
	//qDebug() << "Call called" << d->name;
	QScriptEngine *eng = val.engine();
	if (!eng) {
		qDebug() << "We cannot access the script-engine, fail!";
		return QScriptValue();
	}
	QScriptContext *ctx = eng->currentContext();
	return d->object.property(d->name).call(QScriptValue(), ctx->argumentsObject());
}

