#ifndef VENDORDEVICETREEITEM_H
#define VENDORDEVICETREEITEM_H

#include <QVariant>
#include <QXmlStreamReader>
#include "device.h"

class VendorDeviceTreeItem
{
	Q_DISABLE_COPY(VendorDeviceTreeItem)
public:
	VendorDeviceTreeItem(VendorDeviceTreeItem *parent = 0);
	~VendorDeviceTreeItem();

	void appendChild(VendorDeviceTreeItem *child);

	VendorDeviceTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	VendorDeviceTreeItem *parent();

	bool parseXml( const QString &filename );

	QPixmap image() const;
	int widget() const;
	bool isDevice() const;
	QString deviceModel() const;
	const QString &deviceProtocol() const;

	VendorDeviceTreeItem *findByDevice( const Device &device ) const;

private:
	void parseType( QXmlStreamReader *reader );
	void parseVendor( QXmlStreamReader *reader, VendorDeviceTreeItem *parent );
	void parseDevice( QXmlStreamReader *reader, VendorDeviceTreeItem *parent );
	void skipUnknownElement( QXmlStreamReader *reader );

	QList<VendorDeviceTreeItem *> childItems;

	int settingsWidget;
	QString deviceName, protocol, model;
	QString img;
	VendorDeviceTreeItem *parentItem;
};

#endif // VENDORDEVICETREEITEM_H
