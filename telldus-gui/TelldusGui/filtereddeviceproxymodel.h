#ifndef FILTEREDDEVICEPROXYMODEL_H
#define FILTEREDDEVICEPROXYMODEL_H

#include <QSortFilterProxyModel>

class VendorDeviceModel;

class FilteredDeviceProxyModel : public QSortFilterProxyModel {
	Q_OBJECT
public:
	FilteredDeviceProxyModel( QObject * parent = 0 );
	~FilteredDeviceProxyModel();

	virtual void setSourceModel( VendorDeviceModel * sourceModel );

	void showAll();
	void addFilter( const QString & );

signals:
	void setParameter(const QString &name, const QString &value, const QString &protocol,  const QString &model);

protected:
	virtual bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;
private:
	class PrivateData;
	PrivateData *d;
};

#endif // FILTEREDDEVICEPROXYMODEL_H
