#ifndef EDITDEVICEDIALOG_H
#define EDITDEVICEDIALOG_H

#include <QDialog>
#include <QHash>
#include <QModelIndex>

class VendorDeviceModel;
class DeviceSetting;
class QLabel;
class QStackedLayout;

class EditDeviceDialog : public QDialog
{
	Q_OBJECT
public:
	EditDeviceDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~EditDeviceDialog();

private slots:
	void selectionChanged( const QModelIndex & );

private:
	VendorDeviceModel *model;
	QStackedLayout *settingsLayout;
	QLabel *deviceImage;
	QHash<int, DeviceSetting *> deviceSettings;
};

#endif // EDITDEVICEDIALOG_H
