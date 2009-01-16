#include "tellduscenterapplication.h"
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
	systrayIcon.show();

	//setQuitOnLastWindowClosed( false );
}

TelldusCenterApplication::~TelldusCenterApplication() {
}

void TelldusCenterApplication::showMainWindow() {
	if (!mainWindow) {
		mainWindow = new MainWindow();
	}
	mainWindow->show();
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

void TelldusCenterApplication::showMessage( const QString &title, const QString &message, const QString &detailedMessage ) {
	qDebug() << "showMessage:" << title << message << detailedMessage;
}

void TelldusCenterApplication::eventTriggered( const QString &name, const QString &title ) {
	qDebug() << "eventTriggered:" << name << title;
}

TelldusCenterApplication *TelldusCenterApplication::instance() {
	return (static_cast<TelldusCenterApplication *>(QCoreApplication::instance()));
}
