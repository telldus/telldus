#ifndef SETTINGSPLUGIN_H
#define SETTINGSPLUGIN_H

#include <QScriptExtensionPlugin>

class SettingsPlugin : public QScriptExtensionPlugin {
public:
	SettingsPlugin ( QObject * parent = 0 );
	~SettingsPlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;

private:

};


#endif // SETTINGSPLUGIN_H
