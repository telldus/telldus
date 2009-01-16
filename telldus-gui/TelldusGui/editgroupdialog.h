#ifndef EDITGROUPDIALOG_H
#define EDITGROUPDIALOG_H

#include <QDialog>

class Device;
class DeviceModel;

class EditGroupDialog : public QDialog
{
	Q_OBJECT
public:
	EditGroupDialog(Device *device, DeviceModel *model, QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~EditGroupDialog();

private slots:
	void okClicked();

private:
	Device *device;
	DeviceModel *model;
};

#endif // EDITGROUPDIALOG_H
