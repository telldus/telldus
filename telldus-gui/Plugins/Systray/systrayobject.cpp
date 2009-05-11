#include "systrayobject.h"
#include <QApplication>

SystrayObject::SystrayObject( QObject * parent )
		: QObject(parent),
		icon( new SystrayIcon )
{
	connect(icon, SIGNAL(showEventMessage(QString,QString,QString)), qApp, SIGNAL(showMessage(QString,QString,QString)));
	connect(icon, SIGNAL(triggered()), this, SIGNAL(triggered()));
}

SystrayObject::~SystrayObject() {
}

void SystrayObject::showMessage ( const QString & title, const QString & message, QSystemTrayIcon::MessageIcon i, int millisecondsTimeoutHint ) {
	icon->showMessage( title, message, i, millisecondsTimeoutHint );
}
