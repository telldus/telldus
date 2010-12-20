#include "scriptenvironment.h"
#include "configurationdialog.h"
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QStringList>
#include <QTimerEvent>

#include <QDebug>

class ScriptEnvironment::PrivateData {
public:
	QScriptEngine scriptEngine;
	QHash<int, QScriptValue> intervalHash;
	QHash<int, QScriptValue> timeoutHash;
};

ScriptEnvironment::ScriptEnvironment(QObject *parent) :
	QObject(parent)
{
	d = new PrivateData;

	connect(&d->scriptEngine, SIGNAL(signalHandlerException(const QScriptValue &)), this, SLOT(scriptException(const QScriptValue&)));
	d->scriptEngine.installTranslatorFunctions();

	//Self is our new global object
	QScriptValue self = d->scriptEngine.newQObject(this, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassContents);

	{
		//Copy everything from our old global object
		QScriptValueIterator it(d->scriptEngine.globalObject());
		while (it.hasNext()) {
			it.next();
			self.setProperty(it.scriptName(), it.value(), it.flags());
		}
	}
	self.setProperty("self", self);
	d->scriptEngine.setGlobalObject(self);

	QScriptValue application = d->scriptEngine.newQObject(parent);
	d->scriptEngine.globalObject().setProperty("application", application);

	//Create configuration dialog
	QScriptValue configurationDialogObject = d->scriptEngine.newQObject(new ConfigurationDialog(&d->scriptEngine), QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassContents);
	d->scriptEngine.globalObject().property("application").setProperty("configuration", configurationDialogObject);

	//Collect garbage (ie our old global object)
	d->scriptEngine.collectGarbage();
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

void ScriptEnvironment::timerEvent(QTimerEvent *event) {
	int id = event->timerId();

	QScriptValue expression = d->intervalHash.value(id);
	if (!expression.isValid()) {
		expression = d->timeoutHash.value(id);
		if (expression.isValid()) {
			//Clear oneshot
			this->clearTimeout(id);
		}
	}

	if (expression.isString()) {
		d->scriptEngine.evaluate(expression.toString());
	} else if (expression.isFunction()) {
		expression.call();
	}
}

int ScriptEnvironment::setTimeout(const QScriptValue &expression, int delay) {
	if (expression.isString() || expression.isFunction()) {
		if (delay < 0) {
			delay = 0;
		}
		int timerId = startTimer(delay);
		d->timeoutHash.insert(timerId, expression);
		return timerId;
	}
	return -1;
}

void ScriptEnvironment::clearTimeout(int timerId) {
	killTimer(timerId);
	d->timeoutHash.remove(timerId);
}

int ScriptEnvironment::setInterval(const QScriptValue &expression, int delay) {
	if (expression.isString() || expression.isFunction()) {
		if (delay < 0) {
			delay = 0;
		}
		int timerId = startTimer(delay);
		d->intervalHash.insert(timerId, expression);
		return timerId;
	}
	return -1;
}

void ScriptEnvironment::clearInterval(int timerId) {
	killTimer(timerId);
	d->intervalHash.remove(timerId);
}
