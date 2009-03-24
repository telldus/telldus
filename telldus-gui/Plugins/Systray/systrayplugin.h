#ifndef SYSTRAYPLUGIN_H
#define SYSTRAYPLUGIN_H

#include <QtCore/QObject>
#include <QIcon>
#include "../../TelldusCenter/tellduscenterplugin.h"

class SystrayPluginPrivate;

class SystrayPlugin : public QObject, public TelldusCenterPlugin
{
	Q_OBJECT
	Q_INTERFACES(TelldusCenterPlugin)
public:
	SystrayPlugin( );
	virtual ~SystrayPlugin();

	virtual QIcon iconForPage( const QString &page ) const;
	virtual QString pluginName() const;

	virtual QWidget *widget( const QString &page, QWidget *parent ) const;
	virtual QStringList widgets() const;

private:
	SystrayPluginPrivate *d;
};

#endif // SYSTRAYPLUGIN_H
