#include "vendordevicetreeitem.h"

#include "device.h"
#include <QFile>
#include <QPixmap>
#include <QDebug>
#include <iostream>

VendorDeviceTreeItem::VendorDeviceTreeItem(VendorDeviceTreeItem *parent)
		:settingsWidget(0),
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

	return QVariant();
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

QPixmap VendorDeviceTreeItem::image() const {
	QString filename;
	if (model == "") {
		filename = ":/images/vendors/" + img + ".png";
	} else {
		filename = ":/images/devices/" + img + ".png";
	}
	QPixmap image = QPixmap(filename);
	if (image.isNull()) {
		image = QPixmap( 96, 96 );
		image.fill( Qt::transparent );
	}
	return image;
}

int VendorDeviceTreeItem::widget() const {
	return settingsWidget;
}

bool VendorDeviceTreeItem::isDevice() const {
	return model.length() > 0;
}

QString VendorDeviceTreeItem::deviceModel() const {
	return model;
}

const QString &VendorDeviceTreeItem::deviceProtocol() const {
	return protocol;
}

VendorDeviceTreeItem * VendorDeviceTreeItem::findByDevice( const Device &device ) const {
	foreach( VendorDeviceTreeItem *item, childItems ) {
		if (item->deviceProtocol() == device.protocol() &&
			item->deviceModel() == device.model()) {
			return item;
		}
		VendorDeviceTreeItem *i = item->findByDevice( device );
		if (i) {
			return i;
		}
	}
	return 0;
}

bool VendorDeviceTreeItem::parseXml( const QString &filename ) {
	QFile file(filename);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		std::cerr << "Error: Cannot read file " << qPrintable(filename)
				  << ": " << qPrintable(file.errorString())
				  << std::endl;
		return false;
	}

	QXmlStreamReader reader;
	reader.setDevice(&file);

	reader.readNext();
	while (!reader.atEnd()) {
		if (reader.isStartElement()) {
			if (reader.name() == "devices") { //First element
				reader.readNext();
			} else if (reader.name() == "type") {
				parseType( &reader );
			} else {
				reader.raiseError( QObject::tr("Not a Telldus device-file"));
			}
		} else {
			reader.readNext();
		}
	}

	file.close();
	if (reader.hasError()) {
		std::cerr << "Error: Failed to parse file "
				  << qPrintable(filename) << ": "
				  << qPrintable(reader.errorString()) << std::endl;
		return false;
	} else if (file.error() != QFile::NoError) {
		std::cerr << "Error: Cannot read file " << qPrintable(filename)
				  << ": " << qPrintable(file.errorString())
				  << std::endl;
		return false;
	}

	return true;
}

void VendorDeviceTreeItem::parseType( QXmlStreamReader *reader ) {
	VendorDeviceTreeItem *item = new VendorDeviceTreeItem(this);
	item->deviceName = reader->attributes().value("name").toString();
	appendChild(item);

	reader->readNext();
	while(!reader->atEnd()) {
		if (reader->isEndElement()) {
			reader->readNext();
			break;
		}

		if (reader->isStartElement()) {
			if (reader->name() == "vendor") {
				parseVendor( reader, item );
			} else {
				skipUnknownElement( reader );
			}
		} else {
			reader->readNext();
		}
	}
}

void VendorDeviceTreeItem::parseVendor( QXmlStreamReader *reader, VendorDeviceTreeItem *parent ) {
	VendorDeviceTreeItem *item = new VendorDeviceTreeItem(parent);
	item->deviceName = reader->attributes().value("name").toString();
	item->img = reader->attributes().value("image").toString();
	parent->appendChild(item);

	reader->readNext();
	while(!reader->atEnd()) {
		if (reader->isEndElement()) {
			reader->readNext();
			break;
		}

		if (reader->isStartElement()) {
			if (reader->name() == "device") {
				parseDevice( reader, item );
			} else {
				skipUnknownElement( reader );
			}
		} else {
			reader->readNext();
		}
	}
}

void VendorDeviceTreeItem::parseDevice( QXmlStreamReader *reader, VendorDeviceTreeItem *parent ) {
	QXmlStreamAttributes attributes = reader->attributes();
	VendorDeviceTreeItem *item = new VendorDeviceTreeItem(parent);
	item->model = attributes.value("model").toString();
	item->img = attributes.value("image").toString();
	item->settingsWidget = attributes.value("widget").toString().toInt();
	item->protocol = attributes.value("protocol").toString();
	item->deviceName = reader->readElementText(); //This call must be the last one because it clears the attribute-list
	parent->appendChild(item);

	while(!reader->atEnd()) {
		if (reader->isEndElement()) {
			reader->readNext();
			break;
		}

		if (reader->isStartElement()) {
			skipUnknownElement( reader );
		} else {
			reader->readNext();
		}
	}
}

void VendorDeviceTreeItem::skipUnknownElement( QXmlStreamReader *reader ) {
	reader->readNext();
	while (!reader->atEnd()) {
		if (reader->isEndElement()) {
			reader->readNext();
			break;
		}

		if (reader->isStartElement()) {
			skipUnknownElement( reader );
		} else {
			reader->readNext();
		}
	}
}
