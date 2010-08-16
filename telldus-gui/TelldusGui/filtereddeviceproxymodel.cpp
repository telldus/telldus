#include "filtereddeviceproxymodel.h"
#include "vendordevicemodel.h"
#include "vendordevicetreeitem.h"
#include <QStringList>
#include <QDebug>

class FilteredDeviceProxyModel::PrivateData {
public:
	class Filter;
	VendorDeviceModel * model;
	QList<Filter> filters;
	bool filtered;
};

class FilteredDeviceProxyModel::PrivateData::Filter {
public:
	Filter(QString p, QString t);
	QString protocol, type;
};

FilteredDeviceProxyModel::PrivateData::Filter::Filter(QString p, QString t)
	:protocol(p), type(t)
{}

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

void FilteredDeviceProxyModel::addFilter( const QString &filter ) {
	d->filtered = true;

	QString protocol, type;
	foreach(QString parameter, filter.split(';', QString::SkipEmptyParts)) {
		QString name = parameter.section(':', 0, 0);
		QString value = parameter.section(':', -1);
		if (name == "protocol") {
			protocol = value;
		} else if (name == "type") {
			type = value;
		} else if (name == "method") {
		} else {
			emit setParameter(name, value);
		}
	}
	//Make sure we don't add a duplicate
	for( int i = 0; i < d->filters.size(); ++i) {
		if (d->filters.at(i).protocol != protocol) {
			continue;
		}
		if (d->filters.at(i).type != type) {
			continue;
		}
		return;
	}
	d->filters << PrivateData::Filter(protocol, type);

	invalidateFilter();
}

void FilteredDeviceProxyModel::showAll() {
	d->filtered = false;
	d->filters.clear();
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
		//Check if any children is visible
		int children = d->model->rowCount(index);
		for (int i = 0; i < children; ++i) {
			if (this->filterAcceptsRow(i, index)) {
				//Child visible, parent should be visible
				return true;
			}
		}
		//No children visible, parent should be hidden
		return false;
	}

	for (int i = 0; i < d->filters.size(); ++i) {
		if (item->deviceProtocol() == d->filters.at(i).protocol) {
			QString strModel = item->deviceModel().section(':', 0, 0);
			if (strModel.startsWith("selflearning-")) {
				strModel = "selflearning";
			}
			if (strModel == d->filters.at(i).type) {
				return true;
			}
		}
	}
	return false;
}

