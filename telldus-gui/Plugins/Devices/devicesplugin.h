#ifndef DEVICESPLUGIN_H
#define DEVICESPLUGIN_H

#include <QScriptExtensionPlugin>

class DevicesPlugin : public QScriptExtensionPlugin {
public:
	DevicesPlugin ( QObject * parent = 0 );
	~DevicesPlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;

};

#endif // DEVICESPLUGIN_H
