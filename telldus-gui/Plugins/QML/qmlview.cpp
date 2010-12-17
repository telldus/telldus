#include "qmlview.h"
#include "scriptfunctionwrapper.h"
#include <QDeclarativeContext>
#include <QScriptValueIterator>
#include <QVariant>

class QMLView::PrivateData {
public:
	QDir baseDir;
};

QMLView::QMLView(const QDir &dir, const QScriptValue &object) :
	QDeclarativeView()
{
	d = new PrivateData;
	d->baseDir = dir;

	QDeclarativeContext *context = this->rootContext();
	QScriptValueIterator it(object);

	while (it.hasNext()) {
		it.next();
		if (it.value().isFunction()) {
			context->setContextProperty(it.name(), new ScriptFunctionWrapper(object, it.name(), this));
		} else {
			context->setContextProperty(it.name(), it.value().toVariant());
		}
	}
}

QMLView::~QMLView() {
	delete d;
}

void QMLView::load(const QString &filename) {
	this->setSource(d->baseDir.filePath(filename));
}

void QMLView::setProperty( const QString & name, const QScriptValue &value ) {
	QDeclarativeContext *context = this->rootContext();
	context->setContextProperty(name, value.toVariant());
}
