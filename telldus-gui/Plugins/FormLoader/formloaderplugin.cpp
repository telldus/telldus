#include "formloaderplugin.h"
#include "formloaderobject.h"
#include <QScriptEngine>
#include <QDebug>

class FormLoaderPluginPrivate {
public:
	FormLoaderObject *formloader;
};

FormLoaderPlugin::FormLoaderPlugin ( QObject * parent )
		:QScriptExtensionPlugin( parent )
{
	d = new FormLoaderPluginPrivate;
	d->formloader = 0;
}

FormLoaderPlugin::~FormLoaderPlugin() {
	delete d;
}

void FormLoaderPlugin::initialize ( const QString & key, QScriptEngine * engine ) {
	if (key == "com.telldus.form") {
		d->formloader = new FormLoaderObject(engine, this);

		QScriptValue value = engine->newQObject(d->formloader);
		engine->globalObject().property("com").property("telldus").setProperty("form", value);
	}
}

QStringList FormLoaderPlugin::keys () const {
	return QStringList() << "com.telldus.form";
}

Q_EXPORT_PLUGIN2(FormLoaderInterface, FormLoaderPlugin)
