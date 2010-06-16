#include "deviceview.h"
#include "methodwidget.h"
#include "devicemodel.h"
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QDebug>

class MethodDelegate : public QStyledItemDelegate {
public:
	MethodDelegate( QWidget *parent = 0 );

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

DeviceView::DeviceView(QWidget *parent)
		: QTableView(parent)
{
	setAlternatingRowColors( true );
	setShowGrid( false );
	setSelectionBehavior( QAbstractItemView::SelectRows );
	setSelectionMode( QAbstractItemView::SingleSelection );
	horizontalHeader()->setStretchLastSection( true );
	verticalHeader()->hide();
	setItemDelegate(new MethodDelegate(this)); 
}

void DeviceView::setModel ( QAbstractItemModel * model ) {
	QTableView::setModel( model );
	rowsUpdated(QModelIndex(), 0, model->rowCount()-1);
	connect( model, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(rowsUpdated(const QModelIndex &, int, int)) );
	connect( model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(rowsUpdated(const QModelIndex &, int, int)) );
}

void DeviceView::rowsUpdated ( const QModelIndex & /*parent*/, int start, int end ) {
	DeviceModel *model = qobject_cast<DeviceModel*>( this->model() );
	if (!model) {
		return;
	}

	for (int i = start; i <= end; ++i) {
		QModelIndex index = model->index( i, 2, QModelIndex() );
		this->openPersistentEditor(index);
	}
}

MethodDelegate::MethodDelegate( QWidget *parent )
		:QStyledItemDelegate(parent)
{}

QWidget *MethodDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & /* option */, const QModelIndex &index) const {
	DeviceView *p = qobject_cast<DeviceView *>(this->parent());
	if (!p) {
		return 0;
	}
	DeviceModel *model = qobject_cast<DeviceModel*>( p->model() );
	if (!model) {
		return 0;
	}
	MethodWidget *widget = new MethodWidget(model->device(index), parent);
	return widget;
 }
