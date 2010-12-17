#include "scriptfunctionwrapper.h"

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

QVariant ScriptFunctionWrapper::call() {
	return d->object.property(d->name).call().toVariant();
}
