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

	virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

	Device *device( const QModelIndex & );
	int deviceId( const QModelIndex & );
	
signals:
	void deviceChange(int deviceId, int, int);
	void showMessage( const QString &title, const QString &message, const QString &detailedMessage );
	void eventTriggered( const QString &name, const QString &title );

private slots:
	void deviceChanged( int deviceId, int, int );
	void deviceStateChanged( int deviceId );
	void nameChanged ( int, const QString& );


private:
	int rowForId( int deviceId ) const;
	void triggerCellUpdate(int row, int column);
// 	static void deviceEvent(int deviceId, int method, const char *data, int callbackId, void *context);
	static void WINAPI deviceChangeEvent(int deviceId, int, int, int, void *);
	
// 	void connectDeviceSignals( Device *device ) const;
// 	int deviceId( const QModelIndex &index ) const;
// 	int deviceId( int index ) const;

// 	mutable QHash<int, int> indexToId;
	QList<Device *> devices;
	int deviceChangeCallbackId;
};

#endif // DEVICEMODEL_H
