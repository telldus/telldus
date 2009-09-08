#include "tellduscenterapplication.h"
#include <QMessageBox>
#include <QDir>
#include <QIcon>
#include <QPluginLoader>
#include <QScriptEngine>

#include <QDebug>
#include "tellduscenterplugin.h"
#include "plugintree.h"

class TelldusCenterPlugin;

typedef QList<TelldusCenterPlugin *> PluginList;


class TelldusCenterApplicationPrivate {
public:
	PluginList plugins;
	MainWindow *mainWindow;
	QScriptEngine scriptEngine;
};

TelldusCenterApplication::TelldusCenterApplication(int &argc, char **argv)
		:QtSingleApplication(argc, argv)
{
	d = new TelldusCenterApplicationPrivate;
	connect(this, SIGNAL(messageReceived(const QString &)), this, SLOT(msgReceived(const QString &)));
}

TelldusCenterApplication::~TelldusCenterApplication() {
	qDeleteAll(d->plugins);
	delete d;
}

void TelldusCenterApplication::initialize() {
	d->mainWindow = new MainWindow( );
	
	this->setActivationWindow(d->mainWindow, false);
	
	connect(&d->scriptEngine, SIGNAL(signalHandlerException(const QScriptValue &)), this, SLOT(scriptException(const QScriptValue&)));

	loadPlugins();
	loadScripts();
	loadToolbar();

	emit allDoneLoading();
}

PluginList TelldusCenterApplication::plugins() const {
	if (d->plugins.empty()) {
		return PluginList();
	}
	return d->plugins;
}

QScriptValue TelldusCenterApplication::mainWindow() {
	QScriptValue value = d->scriptEngine.newQObject(d->mainWindow);
	return value;
}

void TelldusCenterApplication::showMainWindow() {
	d->mainWindow->show();
}

bool TelldusCenterApplication::isMainWindowShown() {
	return d->mainWindow->isVisible();
}

#if defined(Q_WS_MAC)
bool TelldusCenterApplication::event(QEvent *event)
{
	switch (event->type()) {
	case QEvent::ApplicationActivate: {
		showMainWindow();
		return true;
	}
	default:
		break;
	}
	return QApplication::event(event);
}
#endif

void TelldusCenterApplication::eventTriggered( const QString &name, const QString &title ) {
	qDebug() << "Systray - eventTriggered:" << name << title;
}

void TelldusCenterApplication::loadPlugins() {
	QDir pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
	bool inBuildDir = false;
	if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release") {
		inBuildDir = true;
	}
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
	}
#endif
	if (!pluginsDir.cd("Plugins")) {
		return;
	}
#if defined(Q_OS_WIN)
	if (inBuildDir) {
		pluginsDir.cdUp();
	}
#endif

	this->setLibraryPaths( QStringList(pluginsDir.absolutePath()) );

	QScriptValue object = d->scriptEngine.newQObject(this);
	d->scriptEngine.globalObject().setProperty("application", object);

	QScriptValue mainWindowObject = d->scriptEngine.newQObject(d->mainWindow);
	d->scriptEngine.globalObject().property("application").setProperty("mainwindow", mainWindowObject);

	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();
		if (plugin) { //Load the plugin
			loadPlugin(plugin);
		}
	}
}

void TelldusCenterApplication::loadPlugin(QObject *plugin) {
	TelldusCenterPlugin *iPlugin = qobject_cast<TelldusCenterPlugin *>(plugin);
	if (!iPlugin) {
		return;
	}
	QObject *qObjectPlugin = qobject_cast<QObject *>(plugin);
	if (qObjectPlugin) {
		if (qObjectPlugin->metaObject()->indexOfSignal( QMetaObject::normalizedSignature("hej()") ) >= 0) {
			connect(qObjectPlugin, SIGNAL(hej()), this, SLOT(aboutQt()));
		}
	}
	d->plugins.append(iPlugin);
}

void TelldusCenterApplication::loadScripts() {
	foreach (QString extension, d->scriptEngine.availableExtensions()) {
		if (extension.startsWith("...")) {
			continue;
		}
		qDebug() << "Loading extension:" << extension;
		d->scriptEngine.importExtension( extension );
		if (d->scriptEngine.hasUncaughtException()) {
			qDebug() << QString("Error in %1:%2:").arg(extension).arg(d->scriptEngine.uncaughtExceptionLineNumber())
					<< d->scriptEngine.uncaughtException().toString();
		}
		d->scriptEngine.clearExceptions();
	}
}

void TelldusCenterApplication::loadToolbar() {
	if (!d->plugins.empty()) {
		QSet<QString> toolbarIcons;
		foreach( TelldusCenterPlugin *plugin, d->plugins ) {
			QStringList widgets = plugin->widgets();
			foreach( QString widget, widgets ) {
				d->mainWindow->addWidget( widget, plugin->iconForPage( PluginTree::page(widget) ), plugin->widget(widget, d->mainWindow) );
			}
		}
	}
}

void TelldusCenterApplication::addWidget( const QString &page, const QString &icon, QWidget *widget ) {
	d->mainWindow->addWidget(page, QIcon(icon), widget);
}

void TelldusCenterApplication::addWidget( const QString &page, const QIcon &icon, QWidget *widget ) {
	d->mainWindow->addWidget(page, icon, widget);
}

TelldusCenterApplication *TelldusCenterApplication::instance() {
	return (static_cast<TelldusCenterApplication *>(QCoreApplication::instance()));
}


void TelldusCenterApplication::scriptException(const QScriptValue & exception) {
	qDebug() << "ScriptException:" << d->scriptEngine.uncaughtExceptionLineNumber() << exception.toString();
	qDebug() << "Backtrace:";
	foreach( QString row, d->scriptEngine.uncaughtExceptionBacktrace() ) {
		qDebug() << row;
	}
	d->scriptEngine.clearExceptions();
}

void TelldusCenterApplication::msgReceived(const QString & message) {
	this->showMainWindow();
}
