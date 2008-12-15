#include "tellduscenterapplication.h"

TelldusCenterApplication::TelldusCenterApplication(int &argc, char **argv)
		:QApplication(argc, argv),
		systrayIcon(this)
{
#if defined(Q_WS_MAC)
	systrayIcon.setIcon(QIcon(":/images/devices-bw.png"));
#else
	systrayIcon.setIcon(QIcon(":/images/devices.png"));
#endif
	systrayIcon.show();

	//setQuitOnLastWindowClosed( false );
}

TelldusCenterApplication::~TelldusCenterApplication() {
}

TelldusCenterApplication *TelldusCenterApplication::instance() {
	return (static_cast<TelldusCenterApplication *>(QCoreApplication::instance()));
}
