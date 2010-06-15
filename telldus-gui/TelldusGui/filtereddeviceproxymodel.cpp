#include "filtereddeviceproxymodel.h"
#include "vendordevicemodel.h"
#include "vendordevicetreeitem.h"
#include <QStringList>
#include <QDebug>

class FilteredDeviceProxyModel::PrivateData {
public:
	VendorDeviceModel * model;
	QString protocol, type;
	bool filtered;
};

FilteredDeviceProxyModel::FilteredDeviceProxyModel( QObject * parent )
		: QSortFilterProxyModel(parent),
		d(new PrivateData)
{
	d->filtered = false;
}

FilteredDeviceProxyModel::~FilteredDeviceProxyModel() {
	delete d;
}

void FilteredDeviceProxyModel::setSourceModel( VendorDeviceModel * sourceModel ) {
	QSortFilterProxyModel::setSourceModel(sourceModel);
	d->model = sourceModel;
}

void FilteredDeviceProxyModel::setFilter( const QString &filter ) {
	d->filtered = true;

	foreach(QString parameter, filter.split(';', QString::SkipEmptyParts)) {
		QString name = parameter.section(':', 0, 0);
		QString value = parameter.section(':', -1);
		if (name == "protocol") {
			d->protocol = value;
		} else if (name == "type") {
			d->type = value;
		} else if (name == "method") {
		} else {
			emit setParameter(name, value);
		}
	}

	invalidateFilter();
}

void FilteredDeviceProxyModel::showAll() {
	d->filtered = false;
	invalidateFilter();
}

bool FilteredDeviceProxyModel::filterAcceptsRow ( int sourceRow, const QModelIndex &parentIndex ) const {
	if (!d->filtered) {
		return true;
	}

	QModelIndex index = d->model->index(sourceRow, 0, parentIndex);
	VendorDeviceTreeItem* item = d->model->item( index );
	if (!item) {
		return true;
	}
	if (!item->isDevice()) {
		//Non devices is always shown
		return true;
	}

	if (item->deviceProtocol() == d->protocol) {
		QString strModel = item->deviceModel().section(':', 0, 0);
		if (strModel.startsWith("selflearning-")) {
			strModel = "selflearning";
		}
		if (strModel == d->type) {
			return true;
		}
	}
	return false;
}

