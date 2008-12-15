#include "devicemodel.h"
#include <telldus-core.h>
#include <QQueue>
#include <QDebug>

DeviceModel::DeviceModel(QObject *parent)
		:QAbstractTableModel(parent)
{
}


DeviceModel::~DeviceModel()
{
}

int DeviceModel::rowCount(const QModelIndex &) const {
	return tdGetNumberOfDevices();
}

int DeviceModel::columnCount(const QModelIndex &) const {
	return 1;
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const {
	if (index.column() != 0) {
		return QVariant();
	}

	if (role == Qt::DisplayRole) {
		int id = deviceId( index );
		char *name = tdGetName( id );
		QString deviceName = QString( name );
		delete name;
		return deviceName;
	}

	return QVariant();
}

bool DeviceModel::removeRows ( int row, int count, const QModelIndex & parent ) {
	beginRemoveRows( parent, row, row + count - 1 );

	QQueue<int> idList;
	for ( int i = row; i < row + count; ++i ) {
		idList.enqueue( deviceId(i) );
	}

	while(!idList.isEmpty()) {
		tdRemoveDevice( idList.dequeue() );
	}
	indexToId.clear(); //Clear the index-to-id cache

	endRemoveRows();
	return true;
}

Device *DeviceModel::newDevice() const {
	Device *device = Device::newDevice();
	connect(device, SIGNAL(deviceAdded(int)), this, SLOT(deviceAdded(int)));
	return device;
}

void DeviceModel::deviceAdded( int id ) {
	int deviceCount = tdGetNumberOfDevices();
	beginInsertRows( QModelIndex(), deviceCount - 1, deviceCount );
	qDebug() << "Ny enhet: " << id;
	endInsertRows();
}

int DeviceModel::deviceId( const QModelIndex &index ) const {
	return deviceId( index.row() );
}

int DeviceModel::deviceId( int index ) const {
	if (indexToId.contains(index)) {
		return indexToId[index];
	}
	int id = tdGetDeviceId( index );
	indexToId[index] = id;
	return id;
}
