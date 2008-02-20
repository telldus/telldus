//
// C++ Interface: icon
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ICON_H
#define ICON_H

#include <QSystemTrayIcon>
#include <QMenu>

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class Icon : public QObject
{
	Q_OBJECT
public:
	Icon();

	~Icon();

protected:
	QSystemTrayIcon i;
	QMenu menu;

protected slots:
	void activated( QSystemTrayIcon::ActivationReason reason );
	void setMenu();
	
private slots:
	void bell();
	void dim();
	void on();
	void off();
	
public slots:
	void configure();

private:
	void addDevice( int index, QMenu *menu );
	static bool hasTelldusSetup();
};

#endif
