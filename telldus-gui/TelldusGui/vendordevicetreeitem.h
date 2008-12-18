#ifndef VENDORDEVICETREEITEM_H
#define VENDORDEVICETREEITEM_H

#include <QVariant>

class VendorDeviceTreeItem
{
public:
	VendorDeviceTreeItem(int id, const QString &displayString, VendorDeviceTreeItem *parent = 0);
	~VendorDeviceTreeItem();

	void appendChild(VendorDeviceTreeItem *child);

	VendorDeviceTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	VendorDeviceTreeItem *parent();

private:
	QList<VendorDeviceTreeItem *> childItems;

	int deviceId;
	QString deviceName;
	VendorDeviceTreeItem *parentItem;
};

#endif // VENDORDEVICETREEITEM_H
