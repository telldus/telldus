#ifndef SYSTRAYOBJECT_H
#define SYSTRAYOBJECT_H

#include <QObject>
#include "systrayicon.h"


class SystrayObject : public QObject
{
	Q_OBJECT
public:
	SystrayObject( QObject * parent = 0 );
	virtual ~SystrayObject();

signals:
	void triggered();

public slots:
	void showMessage ( const QString & title, const QString & message, QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information, int millisecondsTimeoutHint = 10000 );

private:
	SystrayIcon *icon;
};

#endif // SYSTRAYOBJECT_H
