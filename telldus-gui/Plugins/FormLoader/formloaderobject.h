#ifndef FORMLOADEROBJECT_H
#define FORMLOADEROBJECT_H

#include <QObject>

class QScriptEngine;

class FormLoaderObject : public QObject
{
	Q_OBJECT
public:
	FormLoaderObject( QScriptEngine *engine, QObject * parent = 0 );
	virtual ~FormLoaderObject();

public slots:
	void load( const QString &name );

private:
	QScriptEngine *engine;
};

#endif // FORMLOADEROBJECT_H
