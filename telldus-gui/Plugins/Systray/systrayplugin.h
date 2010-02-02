#ifndef SYSTRAYPLUGIN_H
#define SYSTRAYPLUGIN_H

#include <QScriptExtensionPlugin>

class SystrayPluginPrivate;

class SystrayPlugin : public QScriptExtensionPlugin {
public:
	SystrayPlugin ( QObject * parent = 0 );
	~SystrayPlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;

private:
	SystrayPluginPrivate *d;
};


#endif // SYSTRAYPLUGIN_H
