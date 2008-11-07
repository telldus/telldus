//
// C++ Implementation: icon
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "icon.h"
#include <QApplication>
#include <QMenu>
#include <QProcess>
#include <QFile>
#include "telldus-core.h"

Icon::Icon()
 : QObject()
{
	this->setMenu();
 	connect( &i, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activated(QSystemTrayIcon::ActivationReason)));
	i.setIcon( QIcon(":/images/lamp-on.png") );
	i.setToolTip( tr("TellStick") );
	i.show();
}


Icon::~Icon()
{
}


/**
 * @fn Icon::activated( QSystemTrayIcon::ActivationReason reason )
 */
void Icon::activated( QSystemTrayIcon::ActivationReason reason )
{
	if (reason == QSystemTrayIcon::DoubleClick) {
		if (hasTelldusSetup()) {
			configure();
		}
	}
}

/**
 * @fn Icon::bell()
 */
void Icon::bell()
{
	QAction *action = (QAction *) sender();
	int retval = tdBell( action->data().toInt() );
	if (retval != TELLSTICK_SUCCESS) {
		char *errorString = tdGetErrorString( retval );
		i.showMessage( tr("Error"), tr("An error occurred while trying to transmit!\n\n%1").arg(errorString), QSystemTrayIcon::Critical );
		free( errorString );
	}
}

/**
 * @fn Icon::dim()
 */
void Icon::dim()
{
	QAction *action = (QAction *) sender();
	int intId = action->data().toString().section(":", 0, 0).toInt();
	int intLevel = action->data().toString().section(":", 1, 1).toInt();
	//i.showMessage( "", QString::number(intId));
	int retval = tdDim( intId, intLevel );
	if (retval != TELLSTICK_SUCCESS) {
		char *errorString = tdGetErrorString( retval );
		i.showMessage( tr("Error"), tr("An error occurred while trying to transmit!\n\n%1").arg(errorString), QSystemTrayIcon::Critical );
		free( errorString );
	}
}

/**
 * @fn Icon::on()
 */
void Icon::on()
{
	QAction *action = (QAction *) sender();
	int retval = tdTurnOn( action->data().toInt() );
	if (retval != TELLSTICK_SUCCESS) {
		char *errorString = tdGetErrorString( retval );
		i.showMessage( tr("Error"), tr("An error occurred while trying to transmit!\n\n%1").arg(errorString), QSystemTrayIcon::Critical );
		free( errorString );
	}
}

/**
 * @fn Icon::off()
 */
void Icon::off()
{
	QAction *action = (QAction *) sender();
	int retval = tdTurnOff( action->data().toInt() );
	if (retval != TELLSTICK_SUCCESS) {
		char *errorString = tdGetErrorString( retval );
		i.showMessage( tr("Error"), tr("An error occurred while trying to transmit!\n\n%1").arg(errorString), QSystemTrayIcon::Critical );
		free( errorString );
	}
}


/**
 * @fn Icon::configure()
 */
void Icon::configure()
{
	QProcess *process = new QProcess(this);
	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(setMenu()));
	process->setWorkingDirectory(QCoreApplication::applicationDirPath());
	process->start("TelldusSetup.exe");
}


/**
 * @fn Icon::setMenu()
 */
void Icon::setMenu()
{
	menu.clear();

	int intNum = tdGetNumberOfDevices();
	int index = 0;
	while (index < intNum) {
		addDevice(index, &menu);
		index++;
	}

	menu.addSeparator();

	if (hasTelldusSetup()) {
		QAction *configure = menu.addAction(tr("&Configure devices"));
		connect(configure, SIGNAL(triggered()), this, SLOT(configure()));
		configure->setIcon( QIcon(":/images/preferences-system.png") );
	}

	QAction *quit = menu.addAction(tr("&Quit"));
	quit->setIcon( QIcon(":/images/system-log-out.png") );
	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));

	i.setContextMenu(&menu);

}

void Icon::addDevice( int index, QMenu *menu ) {
	int intId = tdGetDeviceId(index);
	QMenu *m = menu->addMenu( tdGetName(intId) );

	int methods = tdMethods(intId);
	if (methods & TELLSTICK_TURNON) {
		QAction *on = m->addAction(tr("&On"));
		on->setIcon(QIcon(":/images/lamp-on.png"));
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
		off->setData( intId );
		off->setIcon(QIcon(":/images/lamp-off.png"));
		connect( off, SIGNAL(triggered()), this, SLOT(off()));
	}
	
	if (methods & TELLSTICK_BELL) {
		QAction *bell = m->addAction(tr("&Bell"));
		bell->setData( intId );
		bell->setIcon(QIcon(":/images/bell.png"));
		connect( bell, SIGNAL(triggered()), this, SLOT(bell()));
	}
}

bool Icon::hasTelldusSetup() {
	return QFile::exists("TelldusSetup.exe");
}
