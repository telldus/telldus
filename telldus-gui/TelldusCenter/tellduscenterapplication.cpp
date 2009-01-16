#include "tellduscenterapplication.h"
#include "telldus-core.h"
#include <QMessageBox>
#include <QMenu>
#include <QDebug>

TelldusCenterApplication::TelldusCenterApplication(int &argc, char **argv)
		:QApplication(argc, argv),
		systrayIcon(this)
{
#if defined(Q_WS_MAC)
	systrayIcon.setIcon(QIcon(":/images/devices-bw.png"));
#else
	systrayIcon.setIcon(QIcon(":/images/devices.png"));
#endif
	connect(&systrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activated(QSystemTrayIcon::ActivationReason)));
	updateSystrayMenu();
	systrayIcon.setContextMenu( &systrayMenu );
	systrayIcon.show();

	//setQuitOnLastWindowClosed( false );
}

TelldusCenterApplication::~TelldusCenterApplication() {
}

void TelldusCenterApplication::showMainWindow() {
	if (!isMainWindowShown()) {
		mainWindow = new MainWindow();
	}
	mainWindow->show();
}

bool TelldusCenterApplication::isMainWindowShown() {
	return !mainWindow.isNull();
}

#if defined(Q_WS_MAC)
bool TelldusCenterApplication::event(QEvent *event)
{
	switch (event->type()) {
	case QEvent::ApplicationActivate: {
		showMainWindow();
		return true;
	}
	default:
		break;
	}
	return QApplication::event(event);
}
#endif

void TelldusCenterApplication::activated( QSystemTrayIcon::ActivationReason reason ) {
	if (reason == QSystemTrayIcon::Trigger) {
	}
}

void TelldusCenterApplication::showMessage( const QString &title, const QString &message, const QString &detailedMessage ) {
	if (isMainWindowShown()) {
		QMessageBox msgBox( mainWindow );
		msgBox.setText( message );
		msgBox.setInformativeText( detailedMessage );
		msgBox.setIcon( QMessageBox::Warning );
		msgBox.setStandardButtons( QMessageBox::Ok );
		msgBox.exec();
	} else {
		systrayIcon.showMessage(title, message, QSystemTrayIcon::Warning);
		qDebug() << "showMessage:" << title << message << detailedMessage;
	}
}

void TelldusCenterApplication::eventTriggered( const QString &name, const QString &title ) {
	qDebug() << "eventTriggered:" << name << title;
}


void TelldusCenterApplication::updateSystrayMenu() {
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

//	i.setContextMenu(&menu);
}

void TelldusCenterApplication::addDevice( int intId ) {
	char *name = tdGetName(intId);
	QString deviceName( name );
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

TelldusCenterApplication *TelldusCenterApplication::instance() {
	return (static_cast<TelldusCenterApplication *>(QCoreApplication::instance()));
}
