#ifndef TOUCHPLUGIN_H
#define TOUCHPLUGIN_H

#include <QtCore/QObject>
#include <QIcon>
#include "../../TelldusCenter/tellduscenterplugin.h"

class TouchPlugin : public QObject, public TelldusCenterPlugin
{
	Q_OBJECT
	Q_INTERFACES(TelldusCenterPlugin)
public:
	virtual QIcon iconForPage( const QString &page ) const;
	virtual void init();
	virtual QString pluginName() const;

	virtual QWidget *widget( const QString &page, QWidget *parent ) const;
	virtual QStringList widgets() const;

signals:
	void hej();

};

#endif // TOUCHPLUGIN_H
