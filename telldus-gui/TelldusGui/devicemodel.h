#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include "device.h"

class DeviceModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	DeviceModel(QObject *parent = 0);
	virtual ~DeviceModel();

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );

	const Device &newDevice() const;

private slots:
	void deviceAdded( int id );

private:
	int deviceId( const QModelIndex &index ) const;
	int deviceId( int index ) const;

	mutable QHash<int, int> indexToId;
};

#endif // DEVICEMODEL_H
