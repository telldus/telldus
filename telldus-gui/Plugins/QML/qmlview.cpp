#include "qmlview.h"
#include <QDeclarativeContext>
#include <QVariant>
#include <QDebug>

class QMLView::PrivateData {
public:
	QDir baseDir;
};

QMLView::QMLView(const QDir &dir) :
	QDeclarativeView()
{
	d = new PrivateData;
	d->baseDir = dir;
}

QMLView::~QMLView() {
	delete d;
}

void QMLView::load(const QString &filename) {
	this->setSource(d->baseDir.filePath(filename));
}

void QMLView::setProperty( const QString & name, const QScriptValue &value ) {
	qDebug() << "Setting property" << name << "to" << value.toString();

	QDeclarativeContext *context = this->rootContext();
	context->setContextProperty(name, value.toVariant());

	//QVariant v = value.toVariant();
	//qDebug() << v;
}
