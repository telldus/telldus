#include "configurationdialog.h"
#include <QDebug>

ConfigurationDialog::ConfigurationDialog(QWidget *parent) :
	QDialog(parent)
{
}

void ConfigurationDialog::open() {
	this->exec();
}
