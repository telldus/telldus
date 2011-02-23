#include "scriptenvironment.h"
#include "configurationdialog.h"
#include <limits>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QScriptContextInfo>
#include <QStringList>
#include <QTimerEvent>

#include <QDebug>

class ScriptEnvironment::TimerObj{
public:
	int originalTimerId;
	qint64 remainingDelay;
	QScriptValue expression;
};

class ScriptEnvironment::PrivateData {
public:
	QScriptEngine *scriptEngine;
	QHash<int, QScriptValue> intervalHash;
	QHash<int, TimerObj*> timeoutHash;
};

ScriptEnvironment::ScriptEnvironment(QObject *parent) :
	QObject(parent)
{
	d = new PrivateData;
	d->scriptEngine = new QScriptEngine();

	connect(d->scriptEngine, SIGNAL(signalHandlerException(const QScriptValue &)), this, SLOT(scriptException(const QScriptValue&)));
	d->scriptEngine->installTranslatorFunctions();

	//Self is our new global object
	QScriptValue self = d->scriptEngine->newQObject(this, QScriptEngine::QtOwnership, QScriptEngine::ExcludeSuperClassContents);

	{
		//Copy everything from our old global object
		QScriptValueIterator it(d->scriptEngine->globalObject());
		while (it.hasNext()) {
			it.next();
			self.setProperty(it.scriptName(), it.value(), it.flags());
		}
	}
	self.setProperty("self", self);
	d->scriptEngine->setGlobalObject(self);

	QScriptValue application = d->scriptEngine->newQObject(parent);
	d->scriptEngine->globalObject().setProperty("application", application);

	//Create configuration dialog
	QScriptValue configurationDialogObject = d->scriptEngine->newQObject(new ConfigurationDialog(d->scriptEngine), QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassContents);
	d->scriptEngine->globalObject().property("application").setProperty("configuration", configurationDialogObject);

	//Collect garbage (ie our old global object)
	d->scriptEngine->collectGarbage();
}

ScriptEnvironment::~ScriptEnvironment() {
	foreach(TimerObj *tim, d->timeoutHash){
		delete tim;
	}
//	delete d->scriptEngine; //This seems to crash for some reason?
	delete d;
}

QDir ScriptEnvironment::currentDir() const {
	QScriptContextInfo info(d->scriptEngine->currentContext()->parentContext());
	QFileInfo fileinfo(info.fileName());
	return fileinfo.dir();
}

QScriptEngine *ScriptEnvironment::engine() const {
	return d->scriptEngine;
}

void ScriptEnvironment::scriptException(const QScriptValue & exception) {
	qDebug() << "ScriptException:" << d->scriptEngine->uncaughtExceptionLineNumber() << exception.toString();
	qDebug() << "Backtrace:";
	foreach( QString row, d->scriptEngine->uncaughtExceptionBacktrace() ) {
		qDebug() << row;
	}
	d->scriptEngine->clearExceptions();
}

void ScriptEnvironment::include(const QString &filename) {
	QDir dir = this->currentDir();

	QFile file(dir.filePath(filename));
	file.open(QFile::ReadOnly);
	QString fileContents = file.readAll();
	file.close();

	QScriptContext *ctx = d->scriptEngine->currentContext();

	ctx->setActivationObject(ctx->parentContext()->activationObject());

	d->scriptEngine->evaluate(fileContents, dir.filePath(filename));
}

void ScriptEnvironment::timerEvent(QTimerEvent *event) {
	int id = event->timerId();

	QScriptValue expression = d->intervalHash.value(id);
	if (!expression.isValid()) {
		qint64 remainingDelay = d->timeoutHash.value(id)->remainingDelay;
		if(remainingDelay > 0){
			qint64 delay = remainingDelay;
			remainingDelay =  delay - std::numeric_limits<int>::max();
			d->timeoutHash.value(id)->remainingDelay = remainingDelay;
			if(remainingDelay > 0){
				return;  //just run same timer again with same interval (max int)
			}

			TimerObj *to = d->timeoutHash.value(id);
			d->timeoutHash.remove(to->originalTimerId);
			int newTimerId = startTimer(delay); //delay differs from last time, start a new timer
			killTimer(id);

			d->timeoutHash.insert(newTimerId, to);
			return;
		}

		expression = d->timeoutHash.value(id)->expression;
		this->clearTimeout(d->timeoutHash.value(id)->originalTimerId);
	}

	if (expression.isString()) {
		d->scriptEngine->evaluate(expression.toString());
	} else if (expression.isFunction()) {
		expression.call();
	}
}

int ScriptEnvironment::setTimeout(const QScriptValue &expression, qint64 delay) {
	if (expression.isString() || expression.isFunction()) {

		if (delay < 0) {
			delay = 0;
		}

		TimerObj *to = new TimerObj;
		to->expression = expression;
		to->remainingDelay = delay - std::numeric_limits<int>::max();
		if(to->remainingDelay > 0){
			delay = std::numeric_limits<int>::max();
		}
		int timerId = startTimer(delay);
		to->originalTimerId = timerId;

		d->timeoutHash.insert(timerId, to);
		return timerId;
	}
	return -1;
}

void ScriptEnvironment::clearTimeout(int timerId) {
	bool found = true;
	if(!d->timeoutHash.contains(timerId) || d->timeoutHash.value(timerId)->originalTimerId != timerId){
		//not original timer id, find the key
		found = false;
		if(d->timeoutHash.count() > 0){
			QHashIterator<int, TimerObj*> i(d->timeoutHash);
			while(i.hasNext()){
				i.next();
				if(i.value()->originalTimerId == timerId){
					timerId = i.key();
					found = true;
					break;
				}
			}
		}
	}

	killTimer(timerId);
	if(!found){
		return;
	}
	delete d->timeoutHash.value(timerId);
	d->timeoutHash.remove(timerId);
}

int ScriptEnvironment::setInterval(const QScriptValue &expression, int delay) {
	//not safe for longer intervals (as setTimeout is)
	//if it has to be done in the future, maybe do something like this:
	//delay/((delay/maxint)+1)=new timer that can be run over and over again, and every ((delay/maxint)+1) time, run "expression"
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
