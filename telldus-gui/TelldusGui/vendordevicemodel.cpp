#include "vendordevicemodel.h"
#include "vendordevicetreeitem.h"
#include "device.h"

VendorDeviceModel::VendorDeviceModel(QObject *parent)
		:QAbstractItemModel(parent),
		rootItem(new VendorDeviceTreeItem(0))
{
	rootItem->parseXml( ":/data/devices.xml" );
}

VendorDeviceModel::~VendorDeviceModel() {
	delete rootItem;
}

int VendorDeviceModel::columnCount(const QModelIndex &parent) const {
	if (parent.isValid()) {
		return static_cast<VendorDeviceTreeItem*>(parent.internalPointer())->columnCount();
	}
	return rootItem->columnCount();
}

QVariant VendorDeviceModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) {
		return QVariant();
	}

	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	VendorDeviceTreeItem *item = static_cast<VendorDeviceTreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags VendorDeviceModel::flags(const QModelIndex &index) const {
	if (!index.isValid()) {
		return 0;
	}

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant VendorDeviceModel::headerData(int , Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return tr("Name");
	}

	return QVariant();
}

QModelIndex VendorDeviceModel::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	VendorDeviceTreeItem *parentItem;

	if (!parent.isValid()) {
		parentItem = rootItem;
	} else {
		parentItem = static_cast<VendorDeviceTreeItem*>(parent.internalPointer());
	}

	VendorDeviceTreeItem *childItem = parentItem->child(row);
	if (childItem) {
		return createIndex(row, column, childItem);
	}
	return QModelIndex();
}

QModelIndex VendorDeviceModel::index(Device *device) const {
	VendorDeviceTreeItem *item = rootItem->findByDeviceId( device->model() );
	if (!item) {
		return QModelIndex();
	}

	return createIndex( item->row(), 0, item );
}

QModelIndex VendorDeviceModel::parent(const QModelIndex &index) const {
	if (!index.isValid()) {
		return QModelIndex();
	}

	VendorDeviceTreeItem *childItem = static_cast<VendorDeviceTreeItem*>(index.internalPointer());
	VendorDeviceTreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem) {
		return QModelIndex();
	}

	return createIndex(parentItem->row(), 0, parentItem);
}

int VendorDeviceModel::rowCount(const QModelIndex &parent) const {
	VendorDeviceTreeItem *parentItem;
	if (parent.column() > 0) {
		return 0;
	}

	if (!parent.isValid()) {
		parentItem = rootItem;
	} else {
		parentItem = static_cast<VendorDeviceTreeItem*>(parent.internalPointer());
	}

	return parentItem->childCount();
}

VendorDeviceTreeItem* VendorDeviceModel::item( const QModelIndex &index ) const {
	if (!index.isValid()) {
		return 0;
	}

	VendorDeviceTreeItem *item = static_cast<VendorDeviceTreeItem*>(index.internalPointer());
	if (item) {
		return item;
	}

	return 0;
}
