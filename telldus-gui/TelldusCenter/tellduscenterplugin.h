#ifndef TELLDUSCENTERPLUGININTERFACE_H
#define TELLDUSCENTERPLUGININTERFACE_H

class TelldusCenterPlugin
{
public:
	virtual ~TelldusCenterPlugin() {}

	virtual QIcon iconForPage( const QString &page ) const = 0;
	virtual QString pluginName() const = 0;
	virtual QWidget *widget( const QString &page, QWidget *parent ) const = 0;
	virtual QStringList widgets() const = 0;
};

Q_DECLARE_INTERFACE(TelldusCenterPlugin, "com.telldus.TelldusCenter.Plugin/1.0")

#endif // TELLDUSCENTERPLUGININTERFACE_H
