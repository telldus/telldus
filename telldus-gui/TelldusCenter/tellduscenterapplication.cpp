#include "tellduscenterapplication.h"
#include <QMessageBox>
#include <QDir>
#include <QIcon>
#include <QPluginLoader>
#include <QTranslator>
#include <QScriptEngine>

#include <QDebug>
#include "tellduscenterplugin.h"
#include "plugintree.h"
#include "scriptenvironment.h"

class TelldusCenterPlugin;

typedef QList<TelldusCenterPlugin *> PluginList;


class TelldusCenterApplicationPrivate {
public:
	PluginList plugins;
	MainWindow *mainWindow;
	ScriptEnvironment *scriptEnvironment;
};

TelldusCenterApplication::TelldusCenterApplication(int &argc, char **argv)
		:QtSingleApplication(argc, argv)
{
	d = new TelldusCenterApplicationPrivate;
	connect(this, SIGNAL(messageReceived(const QString &)), this, SLOT(msgReceived(const QString &)));
	d->scriptEnvironment = new ScriptEnvironment(this);
}

TelldusCenterApplication::~TelldusCenterApplication() {
	qDeleteAll(d->plugins);
	delete d;
}

void TelldusCenterApplication::initialize() {
	d->mainWindow = new MainWindow( d->scriptEnvironment );

	this->setActivationWindow(d->mainWindow, false);

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
	QScriptValue value = d->scriptEnvironment->engine()->newQObject(d->mainWindow);
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

#if defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
	}
#endif
	if (!pluginsDir.cd("Plugins")) {
		return;
	}

	this->setLibraryPaths( QStringList(pluginsDir.absolutePath()) );

	QScriptValue mainWindowObject = d->scriptEnvironment->engine()->newQObject(d->mainWindow);
	d->scriptEnvironment->engine()->globalObject().property("application").setProperty("mainwindow", mainWindowObject);

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
	foreach (QString extension, d->scriptEnvironment->engine()->availableExtensions()) {
		if (extension.startsWith("...")) {
			continue;
		}
// 		qDebug() << "Loading extension:" << extension;
		foreach(QDir dir, this->libraryPaths()) {
			dir.cd("script");
			QString path = extension;
			path.replace('.', '/');
			if (!dir.cd(path)) {
				continue;
			}
			QTranslator *translator = new QTranslator(this);
			if (!translator->load("translation_" + QLocale::system().name(), dir.absolutePath())) {
				delete translator;
				continue;
			}
			this->installTranslator(translator);
		}

		d->scriptEnvironment->engine()->importExtension( extension );
		if (d->scriptEnvironment->engine()->hasUncaughtException()) {
			qDebug() << QString("Error in %1:%2:").arg(extension).arg(d->scriptEnvironment->engine()->uncaughtExceptionLineNumber())
					<< d->scriptEnvironment->engine()->uncaughtException().toString();
		}
		d->scriptEnvironment->engine()->clearExceptions();
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
	QString path;
	QFileInfo info(icon);
	if (info.isRelative()) {
		QDir dir = d->scriptEnvironment->currentDir();
		path = dir.filePath(icon);
	} else {
		path = icon;
	}

	d->mainWindow->addWidget(page, QIcon(path), widget);
}

void TelldusCenterApplication::addWidget( const QString &page, const QIcon &icon, QWidget *widget ) {
	d->mainWindow->addWidget(page, icon, widget);
}

TelldusCenterApplication *TelldusCenterApplication::instance() {
	return (static_cast<TelldusCenterApplication *>(QCoreApplication::instance()));
}

void TelldusCenterApplication::msgReceived(const QString & message) {
	this->showMainWindow();
}
