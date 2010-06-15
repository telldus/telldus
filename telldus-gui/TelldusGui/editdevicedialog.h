#ifndef EDITDEVICEDIALOG_H
#define EDITDEVICEDIALOG_H

#include <QDialog>
#include <QHash>
#include <QModelIndex>

#include <telldus-core.h>

class Device;
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

	class PrivateData;
	PrivateData *d;
};

#endif // EDITDEVICEDIALOG_H
