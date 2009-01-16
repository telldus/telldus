#include "tellduscenterapplication.h"
#include <QMessageBox>
#include <QDebug>

TelldusCenterApplication::TelldusCenterApplication(int &argc, char **argv)
		:QApplication(argc, argv),
		systrayIcon(this)
{
	connect(&systrayIcon, SIGNAL(showEventMessage(const QString &, const QString &, const QString &)), this, SLOT(showMessage(QString,QString,QString)));
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

void TelldusCenterApplication::eventTriggered( const QString &name, const QString &title ) {
	qDebug() << "Systray - eventTriggered:" << name << title;
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

TelldusCenterApplication *TelldusCenterApplication::instance() {
	return (static_cast<TelldusCenterApplication *>(QCoreApplication::instance()));
}
