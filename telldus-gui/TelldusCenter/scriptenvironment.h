#ifndef SCRIPTENVIRONMENT_H
#define SCRIPTENVIRONMENT_H

#include <QObject>
#include <QScriptValue>
#include <QDir>

class ScriptEnvironment : public QObject
{
	Q_OBJECT
public:
	ScriptEnvironment(QObject *parent = 0);
	~ScriptEnvironment();

	QScriptEngine *engine() const;
	QDir currentDir() const;

signals:

public slots:
	void include(const QString &filename);
	int setInterval(const QScriptValue &expression, int delay);
	void clearInterval(int timerId);

	int setTimeout(const QScriptValue &expression, qint64 delay);
	void clearTimeout(int timerId);

protected:
	void timerEvent(QTimerEvent *event);

private slots:
	void scriptException( const QScriptValue &exception );

private:
	class PrivateData;
	PrivateData *d;
	class TimerObj;
};

#endif // SCRIPTENVIRONMENT_H
