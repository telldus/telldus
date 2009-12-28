
#include <QtCore/QCoreApplication>
#include <QDebug>

#include "TelldusCore.h"

void shutdownHandler(int onSignal) {
	qApp->quit();
}

int main(int argc, char **argv) {
	QCoreApplication app(argc, argv);

	/* Install signal traps for proper shutdown */
	signal(SIGTERM, shutdownHandler);
	signal(SIGINT, shutdownHandler);
	
	TelldusCore telldusCore;
	
    return app.exec();
}
