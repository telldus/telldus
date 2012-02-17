#ifndef CONTROLLERSPLUGIN_H
#define CONTROLLERSPLUGIN_H

#include <QScriptExtensionPlugin>

class ControllersPlugin : public QScriptExtensionPlugin {
public:
	ControllersPlugin ( QObject * parent = 0 );
	~ControllersPlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;
};


#endif // CONTROLLERSPLUGIN_H
