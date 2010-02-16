#include "formloaderobject.h"
#include <QApplication>
#include <QWidget>
#include <QScriptEngine>
#include <QUiLoader>
#include <QStringList>
#include <QDir>
#include <QDebug>

FormLoaderObject::FormLoaderObject( QScriptEngine *e, QObject * parent )
		: QObject(parent),
		engine(e)
{
}

FormLoaderObject::~FormLoaderObject() {
}

QWidget *FormLoaderObject::load( const QString &name ) {

	foreach( QString path, qApp->libraryPaths() ) {
		QDir dir(path);
		if (!dir.cd("Forms")) {
			continue;
		}
		if (!dir.cd(name)) {
			continue;
		}
		if (!dir.exists( QString("%1.ui").arg(name) )) {
			qDebug() << "UI-file not found for form" << name;
			continue;
		}
		if (!dir.exists( QString("%1.js").arg(name) )) {
			qDebug() << "JS-file not found for form" << name;
			continue;
		}
		QString scriptFileName( dir.filePath(QString("%1.js").arg(name)) );
		QFile scriptFile(scriptFileName);
		scriptFile.open(QIODevice::ReadOnly);
		engine->evaluate(scriptFile.readAll(), scriptFileName);
		scriptFile.close();

		QUiLoader loader;
		QFile uiFile( dir.filePath(QString("%1.ui").arg(name)) );
		uiFile.open( QIODevice::ReadOnly );
		QWidget *ui = loader.load(&uiFile);
		uiFile.close();

		QScriptValue ctor = engine->evaluate(name);
		QScriptValue scriptUi = engine->newQObject(ui, QScriptEngine::ScriptOwnership);
		QScriptValue calc = ctor.construct(QScriptValueList() << scriptUi);

		return ui;
	}
	qDebug() << "Form" << name << "not found!";
	return 0;
}

