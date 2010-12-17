#ifndef QMLPLUGIN_H
#define QMLPLUGIN_H

#include <QScriptExtensionPlugin>

class QMLPlugin : public QScriptExtensionPlugin {
public:
	QMLPlugin ( QObject * parent = 0 );
	~QMLPlugin ();

	virtual void initialize ( const QString & key, QScriptEngine * engine );
	virtual QStringList keys () const;

private:

};


#endif // SYSTRAYPLUGIN_H
