#include <QtCore/QCoreApplication>

#include "xplinstance.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	/*xPLInstance *instance = */new xPLInstance("telldus", "center");

	return a.exec();
}
