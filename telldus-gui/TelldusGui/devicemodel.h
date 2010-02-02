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
//	virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

	virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );

	virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

	Device *newDevice() const;
	Device *device( const QModelIndex & ) const;

signals:
	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );
	void eventTriggered( const QString &name, const QString &title );
	void deviceChange( int deviceId, int eventType );

private slots:
	void deviceAdded( int id );
	void deviceStateChanged( int deviceId, int newState );

private:
	static void deviceEvent(int deviceId, int method, const char *data, int callbackId, void *context);

	void connectDeviceSignals( Device *device ) const;
	int deviceId( const QModelIndex &index ) const;
	int deviceId( int index ) const;

	mutable QHash<int, int> indexToId;
};

#endif // DEVICEMODEL_H
