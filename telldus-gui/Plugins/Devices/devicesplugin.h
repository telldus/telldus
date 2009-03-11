#ifndef DEVICESPLUGIN_H
#define DEVICESPLUGIN_H

#include <QtCore/QObject>
#include <QIcon>
#include "../../TelldusCenter/tellduscenterplugin.h"

class DevicesPlugin : public QObject, public TelldusCenterPlugin
{
	Q_OBJECT
	Q_INTERFACES(TelldusCenterPlugin)
public:
	virtual QIcon iconForPage( const QString &page ) const;
	virtual QString pluginName() const;

	virtual QWidget *widget( const QString &page, QWidget *parent ) const;
	virtual QStringList widgets() const;

};

#endif // DEVICESPLUGIN_H
