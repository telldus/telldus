#ifndef VENDORDEVICEMODEL_H
#define VENDORDEVICEMODEL_H

#include <QAbstractItemModel>

class VendorDeviceModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	VendorDeviceModel(QObject *parent = 0);
	~VendorDeviceModel();
};

#endif // VENDORDEVICEMODEL_H
