#include "devicemodel.h"
#include "telldusgui.h"
#include <telldus-core.h>
#include <QQueue>
#include <QIcon>
#include <math.h>
#include <QDebug>

#ifdef _WINDOWS
inline double round(double x) {
	return floor(x + 0.5);
}
#endif

const int SUPPORTED_METHODS = TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_BELL /* | TELLSTICK_DIM */ | TELLSTICK_LEARN;


DeviceModel::DeviceModel(QObject *parent)
		:QAbstractTableModel(parent)
{
	connect(this, SIGNAL(deviceChange(int,int,int)), this, SLOT(deviceChanged(int,int,int)));
	int numberOfDevices = tdGetNumberOfDevices();
	for( int i = 0; i < numberOfDevices; ++i ) {
		int id = tdGetDeviceId(i);
		Device *device = new Device(id, SUPPORTED_METHODS, this);
		devices.append(device);
		connect(device, SIGNAL(stateChanged(int)), this, SLOT(deviceStateChanged(int)));
		connect(device, SIGNAL(nameChanged(int,QString)), this, SLOT(nameChanged(int,QString)));
	}
	
	deviceChangeCallbackId = tdRegisterDeviceChangeEvent( reinterpret_cast<TDDeviceChangeEvent>(&DeviceModel::deviceChangeEvent), this);
}


DeviceModel::~DeviceModel()
{
	tdUnregisterCallback(deviceChangeCallbackId);
}

int DeviceModel::rowCount(const QModelIndex &) const {
	return devices.size();
}

int DeviceModel::columnCount(const QModelIndex &) const {
	return 3;
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const {
	if (index.column() > 2) {
		return QVariant();
	}

	if (index.row() >= devices.size()) {
		return QVariant();
	}


	if (index.column() == 0) {
		if (role == Qt::DisplayRole) {
			Device *device = devices[index.row()];
			switch( device->lastSentCommand() ) {
				case TELLSTICK_TURNON:
					return tr("on");
				case TELLSTICK_TURNOFF:
					return tr("off");
				case TELLSTICK_DIM:
					int value = (int)round(device->lastSentValue().toFloat()/255*100);
					return tr("%1%").arg(value);
			}
			return tr("unknown %1").arg(device->lastSentCommand());
		} else if (role == Qt::DecorationRole) {
			Device *device = devices[index.row()];
			int lastSentCommand = device->lastSentCommand();
			return QIcon( QString(":/images/state_%1.png").arg(lastSentCommand) );
 		} else if (role == Qt::TextAlignmentRole) {
 			return Qt::AlignCenter;
 		}
	} else if (index.column() == 1) {
		if (role == Qt::DisplayRole) {
			return devices[index.row()]->name();
		}
// 	} else if (index.column() == 2) {
// 		if (role == Qt::DisplayRole) {
// 			return devices[index.row()]->methods();
// 		}
	}

	return QVariant();
}

QVariant DeviceModel::headerData ( int section, Qt::Orientation orientation, int role ) const {
	if (orientation != Qt::Horizontal) {
		return QVariant();
	}

	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	switch (section) {
		case 0:
			return tr("State");
		case 1:
			return tr("Device name");
		case 2:
			return tr("Action");
	}

	return QVariant();
}

Device *DeviceModel::device( const QModelIndex &index ) {
	if (index.row() >= devices.size()) {
		return 0;
	}
	return devices[index.row()];
}

int DeviceModel::deviceId( const QModelIndex &index ) {
	if (index.row() >= devices.size()) {
		return 0;
	}
	return devices[index.row()]->id();
}

void DeviceModel::deviceStateChanged( int deviceId ) {
	int row = rowForId(deviceId);
	if (row >= 0) {
		triggerCellUpdate(row, 0);
	}
}

void DeviceModel::deviceChanged( int deviceId, int eventType, int changeType ) {
	if (eventType == TELLSTICK_DEVICE_ADDED) {
		int deviceCount = devices.size();
		beginInsertRows( QModelIndex(), deviceCount, deviceCount );
		Device *device = new Device(deviceId, SUPPORTED_METHODS, this);
		devices.append(device);
		connect(device, SIGNAL(stateChanged(int)), this, SLOT(deviceStateChanged(int)));
		connect(device, SIGNAL(nameChanged(int,QString)), this, SLOT(nameChanged(int,QString)));
		endInsertRows();

	} else if (eventType == TELLSTICK_DEVICE_REMOVED) {
		int row = rowForId(deviceId);
		if (row >= 0) {
			beginRemoveRows( QModelIndex(), row, row );
			Device *d = devices.takeAt(row);
			delete d;
			endRemoveRows();
		}
	}
	
}

void DeviceModel::nameChanged(int deviceId, const QString &) {
	int row = rowForId(deviceId);
	if (row >= 0) {
		triggerCellUpdate(row, 1);
	}
}

void DeviceModel::triggerCellUpdate(int row, int column) {
	QModelIndex index = this->index(row, column);
	emit dataChanged( index, index );
}

int DeviceModel::rowForId( int deviceId ) const {
	for (int i = 0; i < devices.size(); ++i) {
		if (devices.at(i)->id() == deviceId) {
			return i;
		}
	}
	return -1;
}


void WINAPI DeviceModel::deviceChangeEvent(int deviceId, int eventId, int changeType, int, void *context) {
	DeviceModel *model = reinterpret_cast<DeviceModel *>(context);
	if (!model) {
		return;
	}
	emit model->deviceChanged(deviceId, eventId, changeType);
}

/*void DeviceModel::connectDeviceSignals( Device *device ) const {
	connect(device, SIGNAL(showMessage(const QString &, const QString &, const QString &)), this, SIGNAL(showMessage(const QString &, const QString &, const QString &)));
	connect(device, SIGNAL(eventTriggered(const QString &, const QString &)), this, SIGNAL(eventTriggered(const QString &, const QString &)));
	connect(device, SIGNAL(stateChanged(int, int)), this, SLOT(deviceStateChanged(int,int)));
	connect(device, SIGNAL(deviceChanged(int,int,int)), this, SLOT(deviceChanged(int,int,int)));
}*/
