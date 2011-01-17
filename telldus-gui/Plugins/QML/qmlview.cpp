#include "qmlview.h"
#include "scriptfunctionwrapper.h"
#include <QDeclarativeContext>
#include <QScriptValueIterator>
#include <QVariant>

class QMLView::PrivateData {
public:
	QDir baseDir;
	QScriptValue object;
};

QMLView::QMLView(const QDir &dir, const QScriptValue &object) :
	QDeclarativeView()
{
	d = new PrivateData;
	d->baseDir = dir;
	d->object = object;

	QScriptValueIterator it(object);

	while (it.hasNext()) {
		it.next();
		this->setProperty(it.name(), it.value());
	}
}

QMLView::~QMLView() {
	delete d;
}

void QMLView::load(const QString &filename) {
	this->setSource(d->baseDir.filePath(filename));
	QSize size = this->initialSize();
	if (size.isNull()) {
		this->setResizeMode(QDeclarativeView::SizeRootObjectToView);
	} else {
		this->setResizeMode(QDeclarativeView::SizeViewToRootObject);
		this->setMinimumSize(size);
	}
}

void QMLView::setProperty( const QString & name, const QScriptValue &value ) {
	QDeclarativeContext *context = this->rootContext();
	if (value.isFunction()) {
		context->setContextProperty(name, new ScriptFunctionWrapper(d->object, name, this));
	} else {
		context->setContextProperty(name, value.toVariant());
	}
}
