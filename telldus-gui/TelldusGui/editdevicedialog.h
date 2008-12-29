#ifndef EDITDEVICEDIALOG_H
#define EDITDEVICEDIALOG_H

#include <QDialog>
#include <QHash>
#include <QModelIndex>

class VendorDeviceModel;
class DeviceSetting;
class Device;
class QLabel;
class QLineEdit;
class QStackedLayout;
class QItemSelectionModel;

class EditDeviceDialog : public QDialog
{
	Q_OBJECT
public:
	EditDeviceDialog(Device *device, QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~EditDeviceDialog();

private slots:
	void selectionChanged( const QModelIndex & );
	void okClicked();

private:
	VendorDeviceModel *model;
	Device *device;
	QStackedLayout *settingsLayout;
	QLabel *deviceImage;
	QLineEdit *nameLineEdit;
	QItemSelectionModel *selection;
	QHash<int, DeviceSetting *> deviceSettings;
};

#endif // EDITDEVICEDIALOG_H
