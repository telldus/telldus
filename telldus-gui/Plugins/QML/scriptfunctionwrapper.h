#ifndef SCRIPTFUNCTIONWRAPPER_H
#define SCRIPTFUNCTIONWRAPPER_H

#include <QObject>
#include <QScriptValue>

class ScriptFunctionWrapper : public QObject
{
	Q_OBJECT
public:
	explicit ScriptFunctionWrapper(const QScriptValue &object, const QString &name, QObject *parent = 0);
	virtual ~ScriptFunctionWrapper();

signals:
	void titleChanged();

public slots:
	QScriptValue call();
	QScriptValue callWith(const QScriptValue &val);


private:
	class PrivateData;
	PrivateData *d;
};

#endif // SCRIPTFUNCTIONWRAPPER_H
