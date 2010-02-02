#include "systrayplugin.h"
#include "systrayobject.h"
#include <QScriptEngine>
#include <QDebug>

class SystrayPluginPrivate {
public:
	SystrayObject *icon;
};

SystrayPlugin::SystrayPlugin ( QObject * parent )
		:QScriptExtensionPlugin( parent )
{
	d = new SystrayPluginPrivate;
	d->icon = 0;
}

SystrayPlugin::~SystrayPlugin() {
	delete d;
}

void SystrayPlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.systray") {
		d->icon = new SystrayObject(engine, this);

		QScriptValue value = engine->newQObject(d->icon);
		engine->globalObject().property("com").property("telldus").setProperty("systray", value);
	}
}

QStringList SystrayPlugin::keys () const {
	return QStringList() << "com.telldus.systray";
}

Q_EXPORT_PLUGIN2(SystrayInterface, SystrayPlugin)
