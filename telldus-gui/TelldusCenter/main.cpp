#include "tellduscenterapplication.h"
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDir>
#include <QDebug>

#ifdef Q_WS_MAC
#include "CocoaInitializer.h"
#include "SparkleAutoUpdater.h"
#endif

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE( resource );

	QCoreApplication::setOrganizationName("Telldus");
	QCoreApplication::setOrganizationDomain("www.telldus.se");
	QCoreApplication::setApplicationName("TelldusCenter");
	
	bool showMinimized = false;
	for( int i = 1; i < argc; ++i ) {
		if (QString(argv[i]) == "--minimized") {
			showMinimized = true;
		}
	}

	TelldusCenterApplication application(argc, argv);
	
	if (application.sendMessage("Wake up!")) {
		return 0;
	}

	QTranslator qtTranslator, tellduscenterTranslator;
#if defined(Q_OS_WIN)
	qtTranslator.load("qt_" + QLocale::system().name());
	tellduscenterTranslator.load("TelldusCenter_" + QLocale::system().name());
#elif defined(Q_OS_MAC)
	QDir resDir = QDir(qApp->applicationDirPath());
	if (resDir.dirName() == "MacOS") {
		resDir.cdUp();
		resDir.cd("resources");
	}
	qtTranslator.load("qt_" + QLocale::system().name(), resDir.absolutePath());
	tellduscenterTranslator.load("TelldusCenter_" + QLocale::system().name(), resDir.absolutePath());
#else
	qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	tellduscenterTranslator.load("TelldusCenter_" + QLocale::system().name());
#endif
	application.installTranslator(&qtTranslator);
	application.installTranslator(&tellduscenterTranslator);
	
	application.initialize();
	
	if (!showMinimized) {
		application.showMainWindow();
	}

#ifdef Q_WS_MAC
/*	AutoUpdater* updater = 0;
	CocoaInitializer initializer;
	updater = new SparkleAutoUpdater("file:///Users/micke/Documents/dev/appcast.xml");
	if (updater) {
		//updater->checkForUpdates();
	}*/
#endif

	return application.exec();
}
