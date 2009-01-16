#ifndef TELLDUSCENTERAPPLICATION_H
#define TELLDUSCENTERAPPLICATION_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QPointer>
#include <QMenu>

#include "mainwindow.h"

class TelldusCenterApplication : public QApplication
{
	Q_OBJECT
public:
	TelldusCenterApplication(int &argc, char **argv);
	virtual ~TelldusCenterApplication();

#if defined(Q_WS_MAC)
	bool event(QEvent *event);
#endif

	void showMainWindow();
	bool isMainWindowShown();

	static TelldusCenterApplication *instance();

public slots:
	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );
	void eventTriggered( const QString &name, const QString &title );

private slots:
	void activated( QSystemTrayIcon::ActivationReason reason );
	void updateSystrayMenu();
	void addDevice( int id );

private:
	QSystemTrayIcon systrayIcon;
	QPointer<MainWindow> mainWindow;
	QMenu systrayMenu;
};

#endif // TELLDUSCENTERAPPLICATION_H
