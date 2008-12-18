#ifndef VENDORDEVICEMODEL_H
#define VENDORDEVICEMODEL_H

#include <QAbstractItemModel>
#include <QList>

class VendorDeviceTreeItem;

class VendorDeviceModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	VendorDeviceModel(QObject *parent = 0);
	~VendorDeviceModel();

	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	int rowCount(const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &index) const;

private:
	VendorDeviceTreeItem *rootItem;
};

#endif // VENDORDEVICEMODEL_H
