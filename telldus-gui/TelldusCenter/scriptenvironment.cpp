#include "scriptenvironment.h"
#include <QScriptEngine>
#include <QStringList>

#include <QDebug>

class ScriptEnvironment::PrivateData {
public:
	QScriptEngine scriptEngine;
};

ScriptEnvironment::ScriptEnvironment(QObject *parent) :
	QObject(parent)
{
	d = new PrivateData;

	connect(&d->scriptEngine, SIGNAL(signalHandlerException(const QScriptValue &)), this, SLOT(scriptException(const QScriptValue&)));
	d->scriptEngine.installTranslatorFunctions();

}

ScriptEnvironment::~ScriptEnvironment() {
	delete d;
}

QScriptEngine *ScriptEnvironment::engine() const {
	return &d->scriptEngine;
}

void ScriptEnvironment::scriptException(const QScriptValue & exception) {
	qDebug() << "ScriptException:" << d->scriptEngine.uncaughtExceptionLineNumber() << exception.toString();
	qDebug() << "Backtrace:";
	foreach( QString row, d->scriptEngine.uncaughtExceptionBacktrace() ) {
		qDebug() << row;
	}
	d->scriptEngine.clearExceptions();
}
