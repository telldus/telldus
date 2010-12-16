#ifndef SCRIPTENVIRONMENT_H
#define SCRIPTENVIRONMENT_H

#include <QObject>
#include <QScriptValue>

class ScriptEnvironment : public QObject
{
	Q_OBJECT
public:
	ScriptEnvironment(QObject *parent = 0);
	~ScriptEnvironment();

	QScriptEngine *engine() const;

signals:

public slots:

private slots:
	void scriptException( const QScriptValue &exception );

private:
	class PrivateData;
	PrivateData *d;

};

#endif // SCRIPTENVIRONMENT_H
