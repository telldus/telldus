#ifndef VENDORDEVICETREEITEM_H
#define VENDORDEVICETREEITEM_H

#include <QVariant>
#include <QXmlStreamReader>

class VendorDeviceTreeItem
{
	Q_DISABLE_COPY(VendorDeviceTreeItem)
public:
	VendorDeviceTreeItem(int id = 0, VendorDeviceTreeItem *parent = 0);
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
	int deviceModel() const;
	const QString &deviceProtocol() const;

	VendorDeviceTreeItem *findByDeviceId( int deviceId ) const;

private:
	void parseType( QXmlStreamReader *reader );
	void parseVendor( QXmlStreamReader *reader, VendorDeviceTreeItem *parent );
	void parseDevice( QXmlStreamReader *reader, VendorDeviceTreeItem *parent );
	void skipUnknownElement( QXmlStreamReader *reader );

	QList<VendorDeviceTreeItem *> childItems;

	int deviceId, settingsWidget;
	QString deviceName, protocol;
	QString img;
	VendorDeviceTreeItem *parentItem;
};

#endif // VENDORDEVICETREEITEM_H
