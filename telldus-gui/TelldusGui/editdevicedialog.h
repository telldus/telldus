#ifndef EDITDEVICEDIALOG_H
#define EDITDEVICEDIALOG_H

#include <QDialog>
#include <QHash>
#include <QModelIndex>

#include <telldus-core.h>

class FilteredDeviceProxyModel;
class VendorDeviceModel;
class DeviceSetting;
class Device;
class QLabel;
class QPushButton;
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

signals:
	void rawDataReceived( const QString & );

private slots:
	void scanClicked();
	void stopScanClicked();
	void selectionChanged( const QModelIndex & );
	void okClicked();
	void cancelClicked();
	void rawDataSlot( const QString &data );

private:
	static void WINAPI rawData(const char *data, int callbackId, void *context);

	void expandNodes(QTreeView *deviceView);
	VendorDeviceModel *model;
	FilteredDeviceProxyModel *filteredModel;
	Device *device;
	QStackedLayout *settingsLayout;
	QLabel *deviceImage;
	QPushButton *scanButton, *stopScanButton;
	QLineEdit *nameLineEdit;
	QItemSelectionModel *selection;
	QHash<int, DeviceSetting *> deviceSettings;
	int rawDeviceEventId;
	bool scanning;
#ifdef Q_WS_HILDON
	QStackedLayout *stacked_layout;
#endif
};

#endif // EDITDEVICEDIALOG_H
