#include "systrayicon.h"
#include "telldus-core.h"
#include "tellduscenterapplication.h"

#include <QApplication>
#include <QDebug>

SystrayIcon::SystrayIcon( QObject * parent )
		:QSystemTrayIcon( parent )
{
#if defined(Q_WS_MAC)
	setIcon(QIcon(":/images/devices-bw.png"));
#else
	setIcon(QIcon(":/images/devices.png"));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activated(QSystemTrayIcon::ActivationReason)));
#endif
	updateSystrayMenu();
	setContextMenu( &systrayMenu );
	show();
}

#if !defined(Q_WS_MAC)
void SystrayIcon::activated( QSystemTrayIcon::ActivationReason reason ) {
	if (reason == QSystemTrayIcon::Trigger) {
		TelldusCenterApplication *app = TelldusCenterApplication::instance();
		app->showMainWindow();
	}
}
#endif

void SystrayIcon::on() {
	QAction *action = qobject_cast<QAction*>(sender());
	if (action) {
		int retval = tdTurnOn( action->data().toInt() );
		if (retval != TELLSTICK_SUCCESS) {
			char *errorString = tdGetErrorString( retval );
			QString message = QString::fromLocal8Bit( errorString );
			free(errorString);
			emit showEventMessage("", message, "");
		}
	}
}

void SystrayIcon::off() {
	QAction *action = qobject_cast<QAction*>(sender());
	if (action) {
		int retval = tdTurnOff( action->data().toInt() );
		if (retval != TELLSTICK_SUCCESS) {
			char *errorString = tdGetErrorString( retval );
			QString message = QString::fromLocal8Bit( errorString );
			free(errorString);
			emit showEventMessage("", message, "");
		}
	}
}

void SystrayIcon::dim() {
	QAction *action = qobject_cast<QAction*>(sender());
	if (action) {
		int intId = action->data().toString().section(":", 0, 0).toInt();
		int intLevel = action->data().toString().section(":", 1, 1).toInt();
		int retval = tdDim( intId, intLevel );
		if (retval != TELLSTICK_SUCCESS) {
			char *errorString = tdGetErrorString( retval );
			QString message = QString::fromLocal8Bit( errorString );
			free(errorString);
			emit showEventMessage("", message, "");
		}
	}
}

void SystrayIcon::bell() {
	QAction *action = qobject_cast<QAction*>(sender());
	if (action) {
		int retval = tdBell( action->data().toInt() );
		if (retval != TELLSTICK_SUCCESS) {
			char *errorString = tdGetErrorString( retval );
			QString message = QString::fromLocal8Bit( errorString );
			free(errorString);
			emit showEventMessage("", message, "");
		}
	}
}

void SystrayIcon::eventTriggered( const QString &name, const QString &title ) {
	qDebug() << "Systray - eventTriggered:" << name << title;
}

void SystrayIcon::updateSystrayMenu() {
	systrayMenu.clear();

	int intNum = tdGetNumberOfDevices();
	int index = 0;
	while (index < intNum) {
		int id = tdGetDeviceId(index);
		addDevice(id);
		index++;
	}

	systrayMenu.addSeparator();

	QAction *quit = systrayMenu.addAction(tr("&Quit"));
	quit->setIcon( QIcon(":/images/system-log-out.png") );
	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void SystrayIcon::addDevice( int intId ) {
	char *name = tdGetName(intId);
	QString deviceName = QString::fromLocal8Bit( name );
	free(name);
	QMenu *m = systrayMenu.addMenu( deviceName );

	int methods = tdMethods(intId, TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM | TELLSTICK_BELL );
	if (methods & TELLSTICK_TURNON) {
		QAction *on = m->addAction(tr("&On"));
		on->setToolTip( tr("Turn on %1").arg( deviceName ) );
		on->setIcon(QIcon(":/images/turn-on.png"));
		on->setData( intId );
		connect( on, SIGNAL(triggered()), this, SLOT(on()));
	}

	if (methods & TELLSTICK_DIM) {
		QAction *dim = m->addAction(tr("90%"));
		dim->setData( QString("%1:230").arg(intId) );
		connect( dim, SIGNAL(triggered()), this, SLOT(dim()));

		dim = m->addAction(tr("80%"));
		dim->setData( QString("%1:204").arg(intId) );
		connect( dim, SIGNAL(triggered()), this, SLOT(dim()));

		dim = m->addAction(tr("70%"));
		dim->setData( QString("%1:179").arg(intId) );
		connect( dim, SIGNAL(triggered()), this, SLOT(dim()));

		dim = m->addAction(tr("60%"));
		dim->setData( QString("%1:153").arg(intId) );
		connect( dim, SIGNAL(triggered()), this, SLOT(dim()));

		dim = m->addAction(tr("50%"));
		dim->setData( QString("%1:128").arg(intId) );
		connect( dim, SIGNAL(triggered()), this, SLOT(dim()));

		dim = m->addAction(tr("40%"));
		dim->setData( QString("%1:102").arg(intId) );
		connect( dim, SIGNAL(triggered()), this, SLOT(dim()));

		dim = m->addAction(tr("30%"));
		dim->setData( QString("%1:77").arg(intId) );
		connect( dim, SIGNAL(triggered()), this, SLOT(dim()));

		dim = m->addAction(tr("20%"));
		dim->setData( QString("%1:51").arg(intId) );
		connect( dim, SIGNAL(triggered()), this, SLOT(dim()));

		dim = m->addAction(tr("10%"));
		dim->setData( QString("%1:25").arg(intId) );
		connect( dim, SIGNAL(triggered()), this, SLOT(dim()));
	}

	if (methods & TELLSTICK_TURNON) {
		QAction *off = m->addAction(tr("O&ff"));
		off->setToolTip( tr("Turn off %1").arg( deviceName ) );
		off->setData( intId );
		off->setIcon(QIcon(":/images/turn-off.png"));
		connect( off, SIGNAL(triggered()), this, SLOT(off()));
	}

	if (methods & TELLSTICK_BELL) {
		QAction *bell = m->addAction(tr("&Bell"));
		bell->setData( intId );
		bell->setIcon(QIcon(":/images/bell.png"));
		connect( bell, SIGNAL(triggered()), this, SLOT(bell()));
	}
}
