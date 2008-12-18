#ifndef EDITDEVICEDIALOG_H
#define EDITDEVICEDIALOG_H

#include <QDialog>

class VendorDeviceModel;

class EditDeviceDialog : public QDialog
{
public:
	EditDeviceDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~EditDeviceDialog();

private:
	VendorDeviceModel *model;
};

#endif // EDITDEVICEDIALOG_H
