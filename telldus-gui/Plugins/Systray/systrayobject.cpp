#include "systrayobject.h"
#include <QApplication>
#include <QMenu>
#include <QDebug>

class SystrayObjectPrivate {
public:
	QSystemTrayIcon *i;
	QAction *menu;
	QHash<int, QAction *> menuItems;
	QScriptEngine *engine;
	int lastMenuId;
};

SystrayObject::SystrayObject( QScriptEngine *engine, QObject * parent )
		: QObject(parent)
{
	d = new SystrayObjectPrivate;
	d->lastMenuId = 0;
	d->engine = engine;
	d->menu = new QAction(0);
	d->menu->setMenu( new QMenu() );
	
	d->i = new QSystemTrayIcon();
	d->i->setIcon(QIcon(":/images/TelldusCenter_128.png"));
	d->i->setContextMenu(d->menu->menu());
	d->i->show();
	
	connect(d->i, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activated(QSystemTrayIcon::ActivationReason)));
	connect(qApp, SIGNAL(aboutToQuit()), d->i, SLOT(hide()));
}

SystrayObject::~SystrayObject() {
	delete d->i;
	delete d->menu;
	delete d;
}

int SystrayObject::addMenuItem( const QString &name, int parent ) {
	QMenu *menu = parentMenu(parent);
	if (!menu) {
		return -1;
	}
	QAction *item = menu->addAction( name );
	int id = ++d->lastMenuId;
	d->menuItems[id]= item;
	return id;
}

int SystrayObject::addMenuItemAfter(const QString & name, int id) {
	QAction *parent = menuAction(id);
	QMenu *menu = qobject_cast<QMenu *>(parent->parentWidget());
	if (!menu) {
		return -1;
	}
	
	QAction *before = 0;
	bool found = false;
	foreach(QAction *action, menu->actions()) {
		if (found) {
			before = action;
			break;
		}
		if (action == parent) {
			found = true;
			continue;
		}
	}
	QAction *item = new QAction( name, menu );
	menu->insertAction( before, item );
	int newId = ++d->lastMenuId;
	d->menuItems[newId] = item;
	return newId;
}

void SystrayObject::addSeparator( int parent ) {
	QMenu *menu = parentMenu(parent);
	if (!menu) {
		return;
	}
	menu->addSeparator();
}

void SystrayObject::clear(int parent) {
	QAction *parentA = menuAction( parent );
	if (!parentA) {
		return;
	}
	QMenu *menu = parentA->menu();
	if (!menu) { //Has no children
		return;
	}
	foreach( QAction *action, menu->actions() ) {
		int key = d->menuItems.key(action, -1);
		if (key >= 0) {
			clear(key);
			d->menuItems.remove(key);
		}
	}
	menu->clear();
}

QScriptValue SystrayObject::menuItem( int id ) {
	QScriptValue value = d->engine->newQObject(menuAction(id));
	return value;
}

void SystrayObject::removeMenuItem(int id) {
	//First we clear the item from submenus
	clear(id);
	
	QAction *action = menuAction( id );
	if (!action) {
		return;
	}
	d->menuItems.remove(id);
	delete action;
}


void SystrayObject::showMessage ( const QString & title, const QString & message, QSystemTrayIcon::MessageIcon i, int millisecondsTimeoutHint ) {
	d->i->showMessage( title, message, i, millisecondsTimeoutHint );
}

void SystrayObject::activated( QSystemTrayIcon::ActivationReason reason ) {
#if !defined(Q_WS_MAC)
	if (reason == QSystemTrayIcon::Trigger) {
		emit triggered();
	}
#endif
}

QAction *SystrayObject::menuAction( int index ) {
	QAction *parentItem;
	if (index < 0) {
		return d->menu;
	}
	return d->menuItems[index];
}

QMenu *SystrayObject::parentMenu( int parentIndex ) {
	QAction *parentItem = menuAction( parentIndex );
	if (!parentItem) {
		return 0;
	}
	QMenu *menu = parentItem->menu();
	if (!menu) {
		menu = new QMenu();
		parentItem->setMenu(menu);
	}
	return menu;
}