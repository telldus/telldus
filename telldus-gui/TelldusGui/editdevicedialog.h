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
class QTreeView;

class EditDeviceDialog : public QDialog
{
	Q_OBJECT
public:
	EditDeviceDialog(Device *device, QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~EditDeviceDialog();

private slots:
	void selectionChanged( const QModelIndex & );
	void okClicked();
	void cancelClicked();

private:
	void expandNodes(QTreeView *deviceView);
	VendorDeviceModel *model;
	Device *device;
	QStackedLayout *settingsLayout;
	QLabel *deviceImage;
	QLineEdit *nameLineEdit;
	QItemSelectionModel *selection;
	QHash<int, DeviceSetting *> deviceSettings;
#ifdef Q_WS_HILDON
	QStackedLayout *stacked_layout;
#endif
};

#endif // EDITDEVICEDIALOG_H
