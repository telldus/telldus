#include <QtCore>
#include <QLabel>
#include <QApplication>
#include "systrayplugin.h"

#include "../../TelldusGui/telldusgui.h"
#include "systrayicon.h"

class SystrayPluginPrivate {
public:
	SystrayIcon *icon;
};

SystrayPlugin::SystrayPlugin( )
		: QObject(),
		TelldusCenterPlugin()
{
	d = new SystrayPluginPrivate;
	d->icon = new SystrayIcon(this);
	qApp->setQuitOnLastWindowClosed( false );
}

SystrayPlugin::~SystrayPlugin() {
	delete d;
}

QIcon SystrayPlugin::iconForPage( const QString &page ) const {
	return QIcon(":/images/devices.png");
}

QString SystrayPlugin::pluginName() const {
	return "Systray icon";
}

QWidget *SystrayPlugin::widget( const QString &page, QWidget *parent ) const {
	return 0;
}

QStringList SystrayPlugin::widgets() const {
	return QStringList();
}

Q_EXPORT_PLUGIN2(SystrayInterface, SystrayPlugin)
