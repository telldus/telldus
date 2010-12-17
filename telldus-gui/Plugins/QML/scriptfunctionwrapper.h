#ifndef SCRIPTFUNCTIONWRAPPER_H
#define SCRIPTFUNCTIONWRAPPER_H

#include <QObject>
#include <QScriptValue>
#include <QVariant>

class ScriptFunctionWrapper : public QObject
{
	Q_OBJECT
public:
	explicit ScriptFunctionWrapper(const QScriptValue &object, const QString &name, QObject *parent = 0);
	virtual ~ScriptFunctionWrapper();

signals:
	void titleChanged();

public slots:
	QVariant call();

private:
	class PrivateData;
	PrivateData *d;
};

#endif // SCRIPTFUNCTIONWRAPPER_H
