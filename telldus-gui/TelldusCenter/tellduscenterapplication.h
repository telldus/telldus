#ifndef TELLDUSCENTERAPPLICATION_H
#define TELLDUSCENTERAPPLICATION_H

#include <QApplication>
#include <QPointer>
//#incldue

#include "systrayicon.h"
#include "mainwindow.h"

class TelldusCenterPlugin;

typedef QList<TelldusCenterPlugin *> PluginList;

class TelldusCenterApplication : public QApplication
{
	Q_OBJECT
public:
	TelldusCenterApplication(int &argc, char **argv);
	virtual ~TelldusCenterApplication();

#if defined(Q_WS_MAC)
	bool event(QEvent *event);
#endif

	PluginList plugins() const;
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
	void loadPlugins();
	void loadPlugin(QObject *plugin);

	SystrayIcon systrayIcon;
	PluginList p_plugins;
	QPointer<MainWindow> mainWindow;
};

#endif // TELLDUSCENTERAPPLICATION_H
