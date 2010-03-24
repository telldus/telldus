#include "deviceview.h"
#include "methodwidget.h"
#include "devicemodel.h"
#include <QHeaderView>
#include <QDebug>

DeviceView::DeviceView(QWidget *parent)
		: QTableView(parent)
{
	setAlternatingRowColors( true );
	setShowGrid( false );
	setSelectionBehavior( QAbstractItemView::SelectRows );
	horizontalHeader()->setStretchLastSection( true );
	verticalHeader()->hide();
}

void DeviceView::setModel ( QAbstractItemModel * model ) {
	QTableView::setModel( model );
	rowsInserted(QModelIndex(), 0, model->rowCount()-1);
	connect( model, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(rowsInserted(const QModelIndex &, int, int)) );
}

void DeviceView::rowsInserted ( const QModelIndex & /*parent*/, int start, int end ) {
	DeviceModel *model = qobject_cast<DeviceModel*>( this->model() );
	if (!model) {
		return;
	}

	for (int i = start; i <= end; ++i) {
		QModelIndex index = model->index( i, 2, QModelIndex() );
		this->setIndexWidget( index, new MethodWidget( model->device(index), this ) );
	}
}
