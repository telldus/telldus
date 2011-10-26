#ifndef SENSORSPLUGIN_H
#define SENSORSPLUGIN_H

#include <QScriptExtensionPlugin>

class SensorsPlugin : public QScriptExtensionPlugin {
public:
	SensorsPlugin ( QObject * parent = 0 );
	~SensorsPlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;
};


#endif // SENSORSPLUGIN_H
