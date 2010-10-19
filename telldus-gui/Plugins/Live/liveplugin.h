#ifndef LIVEPLUGIN_H
#define LIVEPLUGIN_H

#include <QScriptExtensionPlugin>

class LivePlugin : public QScriptExtensionPlugin {
public:
	LivePlugin ( QObject * parent = 0 );
	~LivePlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;
};


#endif // LIVEPLUGIN_H
