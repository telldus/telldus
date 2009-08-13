#ifndef TELLDUSCOREPLUGIN_H
#define TELLDUSCOREPLUGIN_H

#include <QScriptExtensionPlugin>

class TelldusCorePlugin : public QScriptExtensionPlugin {
public:
	TelldusCorePlugin ( QObject * parent = 0 );
	~TelldusCorePlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;
};


#endif // TELLDUSCOREPLUGIN_H
