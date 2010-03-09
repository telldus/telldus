#ifndef XPLPLUGIN_H
#define XPLPLUGIN_H

#include <QScriptExtensionPlugin>

class XPLPlugin : public QScriptExtensionPlugin {
public:
	XPLPlugin ( QObject * parent = 0 );
	~XPLPlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;
};


#endif // XPLPLUGIN_H
