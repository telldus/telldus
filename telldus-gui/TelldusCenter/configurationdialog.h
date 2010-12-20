#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>

class ConfigurationDialog : public QDialog
{
	Q_OBJECT
public:
	explicit ConfigurationDialog(QWidget *parent = 0);

signals:

public slots:
	void open();

};

#endif // CONFIGURATIONDIALOG_H
