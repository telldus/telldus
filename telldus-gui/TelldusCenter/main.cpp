#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE( resource );

	QApplication a(argc, argv);
	MainWindow *w = new MainWindow();
	w->show();
	return a.exec();
}
