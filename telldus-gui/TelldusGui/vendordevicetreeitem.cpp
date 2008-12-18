#include "vendordevicetreeitem.h"

VendorDeviceTreeItem::VendorDeviceTreeItem(int id, const QString &displayString, VendorDeviceTreeItem *parent)
		:deviceId(id),
		deviceName(displayString),
		parentItem(parent)
{
}

VendorDeviceTreeItem::~VendorDeviceTreeItem() {
	qDeleteAll(childItems);
}

void VendorDeviceTreeItem::appendChild(VendorDeviceTreeItem *item) {
	childItems.append(item);
}

VendorDeviceTreeItem *VendorDeviceTreeItem::child(int row) {
	return childItems.value(row);
}

int VendorDeviceTreeItem::childCount() const {
	return childItems.count();
}
int VendorDeviceTreeItem::columnCount() const {
	return 1;
}

QVariant VendorDeviceTreeItem::data(int column) const {
	if (column == 0) {
		return deviceName;
	}

	return "Da";
}

VendorDeviceTreeItem *VendorDeviceTreeItem::parent() {
	return parentItem;
}

int VendorDeviceTreeItem::row() const {
	if (parentItem) {
		return parentItem->childItems.indexOf(const_cast<VendorDeviceTreeItem*>(this));
	}

	return 0;
}
