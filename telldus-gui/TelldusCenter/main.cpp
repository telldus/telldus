#include "tellduscenterapplication.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE( resource );

	TelldusCenterApplication application(argc, argv);

	MainWindow *w = new MainWindow();
	w->show();

	return application.exec();
}
