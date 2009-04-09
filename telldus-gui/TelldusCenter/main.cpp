#include "tellduscenterapplication.h"

#ifdef Q_WS_MAC
#include "CocoaInitializer.h"
#include "SparkleAutoUpdater.h"
#endif

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE( resource );

	QCoreApplication::setOrganizationName("Telldus");
	QCoreApplication::setOrganizationDomain("www.telldus.se");
	QCoreApplication::setApplicationName("Telldus Center");


	TelldusCenterApplication application(argc, argv);

	application.showMainWindow();

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
