#ifndef EDITGROUPDIALOG_H
#define EDITGROUPDIALOG_H

#include <QDialog>
#include <QModelIndex>

class Device;
class DeviceModel;
class ProxyModel;
class QToolButton;
class QTableView;

class EditGroupDialog : public QDialog
{
	Q_OBJECT
public:
	EditGroupDialog(Device *device, DeviceModel *model, QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~EditGroupDialog();

private slots:
	void availableListActivated(const QModelIndex &);
	void addedListActivated(const QModelIndex &);
	void addClicked();
	void removeClicked();
	void okClicked();

private:
	QToolButton *addToolButton, *removeToolButton;
	QTableView *availableListView, *addedListView;
	Device *device;
	DeviceModel *model;
	ProxyModel *availableProxyModel, *addedProxyModel;
};

#endif // EDITGROUPDIALOG_H
