#ifndef FORMLOADERPLUGIN_H
#define FORMLOADERPLUGIN_H

#include <QScriptExtensionPlugin>

class FormLoaderPluginPrivate;

class FormLoaderPlugin : public QScriptExtensionPlugin {
public:
	FormLoaderPlugin ( QObject * parent = 0 );
	~FormLoaderPlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;

private:
	FormLoaderPluginPrivate *d;
};


#endif // FORMLOADERPLUGIN_H
