#ifndef SYSTRAYOBJECT_H
#define SYSTRAYOBJECT_H

#include <QObject>
#include <QScriptEngine>
#include "systrayicon.h"

class SystrayObjectPrivate;

class SystrayObject : public QObject
{
	Q_OBJECT
public:
	SystrayObject( QScriptEngine *engine, QObject * parent = 0 );
	virtual ~SystrayObject();

signals:
	void triggered();

public slots:
	int addMenuItem( const QString &name, int parent = -1 );
	int addMenuItemAfter( const QString &name, int id );
	void addSeparator( int parent = -1 );
	void clear( int parent = -1 );
	QScriptValue menuItem( int id );
	void removeMenuItem( int id );
	
	void showMessage ( const QString & title, const QString & message, QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information, int millisecondsTimeoutHint = 10000 );

private slots:
	void activated( QSystemTrayIcon::ActivationReason reason );

private:
	QAction *menuAction( int index );
	QMenu *parentMenu( int parentIndex );
	SystrayObjectPrivate *d;
};

#endif // SYSTRAYOBJECT_H
