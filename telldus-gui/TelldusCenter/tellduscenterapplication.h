#ifndef TELLDUSCENTERAPPLICATION_H
#define TELLDUSCENTERAPPLICATION_H

#include <QApplication>
#include <QPointer>

#include "systrayicon.h"
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

signals:
	void sigDeviceEvent(int deviceId, int method, const QString &data);

public slots:
	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );
	void eventTriggered( const QString &name, const QString &title );

private slots:
	void deviceEvent(int deviceId, int method, const QString &data);

private:
	static void deviceEvent(int deviceId, int method, const char *data, int callbackId, void *context);

	SystrayIcon systrayIcon;
	QPointer<MainWindow> mainWindow;
};

#endif // TELLDUSCENTERAPPLICATION_H
