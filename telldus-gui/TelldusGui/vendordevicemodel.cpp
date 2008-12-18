#include "vendordevicemodel.h"
#include "vendordevicetreeitem.h"

VendorDeviceModel::VendorDeviceModel(QObject *parent)
		:QAbstractItemModel(parent),
		rootItem(new VendorDeviceTreeItem(0, ""))
{
	rootItem->appendChild( new VendorDeviceTreeItem(0, "Ikea", rootItem) );
	rootItem->appendChild( new VendorDeviceTreeItem(0, "Nexa", rootItem) );
	rootItem->appendChild( new VendorDeviceTreeItem(0, "Proove", rootItem) );
	rootItem->appendChild( new VendorDeviceTreeItem(0, "Sartano", rootItem) );
	rootItem->appendChild( new VendorDeviceTreeItem(0, "Waveman", rootItem) );
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

QVariant VendorDeviceModel::headerData(int section, Qt::Orientation orientation, int role) const {
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
