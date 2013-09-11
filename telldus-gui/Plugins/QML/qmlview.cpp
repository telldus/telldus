#include "qmlview.h"
#include "scriptfunctionwrapper.h"
#include <QDeclarativeContext>
#include <QScriptValueIterator>
#include <QDeclarativeEngine>
#include <QVariant>
#include <QApplication>

#define DEFINE_STRING_HELPER(X) #X
#define DEFINE_STRING(X) DEFINE_STRING_HELPER(X)

class QMLView::PrivateData {
public:
	QDir baseDir;
	QScriptValue object;
};

QMLView::QMLView(const QDir &dir, const QScriptValue &object) :
	QDeclarativeView()
{
	setAttribute(Qt::WA_TranslucentBackground);
	setStyleSheet("background:transparent;");

	QDeclarativeEngine *eng = this->engine();
	QStringList paths(eng->importPathList());
	QDir pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
	}
#endif

	if (pluginsDir.cd("Plugins/declarative")) {
		paths << pluginsDir.absolutePath();
	}

#if defined(Q_OS_UNIX)
	paths << DEFINE_STRING(PLUGIN_LIB_PATH);
#endif

	eng->setImportPathList(paths);

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

void QMLView::sizeRootObjectToView(bool enable) {
	if (enable) {
		this->setResizeMode(QDeclarativeView::SizeRootObjectToView);
	} else {
		this->setResizeMode(QDeclarativeView::SizeViewToRootObject);
	}
}
