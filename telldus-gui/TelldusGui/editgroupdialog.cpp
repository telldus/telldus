#include "editgroupdialog.h"
#include "device.h"
#include "devicemodel.h"

#include <QVBoxLayout>
#include <QListView>
#include <QTableView>
#include <QHeaderView>
#include <QSortFilterProxyModel>

#include <QToolButton>
#include <QApplication>
#include <QDialogButtonBox>
#include <QDebug>

#include <QLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>


class ProxyModel : public QSortFilterProxyModel {
public:
	ProxyModel( QObject * parent = 0 );
	~ProxyModel();

	virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	void showAllRows();
	void hideRow( int row );
	void showRow( int row );
	QString getShownIds();

protected:
	virtual bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;
private:
	QSet<int> rows;
};

class EditGroupDialog::PrivateData {
public:
	QLineEdit *nameLineEdit;
	
};


EditGroupDialog::EditGroupDialog(Device *devicein, DeviceModel *m, QWidget *parent, Qt::WFlags flags)
		:QDialog(parent, flags),
		device(devicein),
		model(m),
		d(new PrivateData)
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	QHBoxLayout *mainLayout = new QHBoxLayout;

	QHBoxLayout *nameLayout = new QHBoxLayout;
	QLabel *nameLabel = new QLabel(this);
	nameLabel->setText( tr("&Name:") );
	d->nameLineEdit = new QLineEdit(device->name(),  this );
	nameLabel->setBuddy(d->nameLineEdit);
	nameLayout->addWidget(nameLabel);
	nameLayout->addWidget(d->nameLineEdit);
	nameLayout->addStretch();
	layout->addLayout(nameLayout);
	
	availableProxyModel = new ProxyModel( this );
	availableProxyModel->setSourceModel( model );
	availableProxyModel->showAllRows();

	availableListView = new QTableView( this );
	availableListView->setAlternatingRowColors( true );
	availableListView->setShowGrid( false );
	availableListView->setSelectionBehavior( QAbstractItemView::SelectRows );
	availableListView->horizontalHeader()->setStretchLastSection( true );
	availableListView->verticalHeader()->hide();
	availableListView->setModel( availableProxyModel );
	availableListView->resizeColumnsToContents();
	connect( availableListView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(availableListActivated(const QModelIndex &)) );
	mainLayout->addWidget( availableListView );

	QVBoxLayout *buttonsLayout = new QVBoxLayout;
	buttonsLayout->addStretch();

	addToolButton = new QToolButton( this );
	addToolButton->setIcon( qApp->style()->standardIcon( QStyle::SP_ArrowRight) );
	addToolButton->setEnabled( false );
	connect(addToolButton, SIGNAL(clicked()), this, SLOT(addClicked()));
	buttonsLayout->addWidget( addToolButton );

	removeToolButton = new QToolButton( this );
	removeToolButton->setIcon( qApp->style()->standardIcon( QStyle::SP_ArrowLeft) );
	removeToolButton->setEnabled( false );
	connect(removeToolButton, SIGNAL(clicked()), this, SLOT(removeClicked()));
	buttonsLayout->addWidget( removeToolButton );

	buttonsLayout->addStretch();
	mainLayout->addLayout( buttonsLayout );

	addedProxyModel = new ProxyModel( this );
	addedProxyModel->setSourceModel( model );
	setDevicesVisible();

	addedListView = new QTableView( this );
	addedListView->setAlternatingRowColors( true );
	addedListView->setShowGrid( false );
	addedListView->setSelectionBehavior( QAbstractItemView::SelectRows );
	addedListView->horizontalHeader()->setStretchLastSection( true );
	addedListView->verticalHeader()->hide();
	addedListView->setModel( addedProxyModel );
	addedListView->resizeColumnsToContents();
	connect( addedListView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(addedListActivated(const QModelIndex &)) );
	mainLayout->addWidget( addedListView );

	layout->addLayout( mainLayout );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
	buttonBox->setStandardButtons( QDialogButtonBox::Save | QDialogButtonBox::Cancel );
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(okClicked()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	layout->addWidget(buttonBox);

}

EditGroupDialog::~EditGroupDialog() {
}

void EditGroupDialog::setDevicesVisible(){
	QString devicesstring = device->parameter("devices", "");
	QList<int> devices;
	foreach(QString device, devicesstring.split(",")) {
		devices << device.toInt();
	}

	for (int i = 0; i < model->rowCount(); ++i){
		int id = model->deviceId(model->index(i, 0));
		if (id == device->id()) {
			//shouldnt be able to add itself, check that here
			availableProxyModel->hideRow(i);
		} else if (devices.contains(id)) {
			availableProxyModel->hideRow(i);
			addedProxyModel->showRow(i);
		}
	}

	
}

void EditGroupDialog::addClicked() {
	int row = availableProxyModel->mapToSource( availableListView->currentIndex() ).row();
	availableProxyModel->hideRow(row);

	addedProxyModel->showRow(row);
}

void EditGroupDialog::removeClicked() {
	int row = addedProxyModel->mapToSource( addedListView->currentIndex() ).row();
	availableProxyModel->showRow(row);
	addedProxyModel->hideRow(row);
}

void EditGroupDialog::okClicked() {

	if (d->nameLineEdit->text().trimmed() == "") {
		QMessageBox msgBox;
		msgBox.setText( tr("The device must have a name.") );
		msgBox.setInformativeText( tr("Please fill in a name in the field under 'Name'") );
		msgBox.setIcon( QMessageBox::Critical );
		msgBox.setStandardButtons( QMessageBox::Ok );
		msgBox.exec();
		d->nameLineEdit->setFocus();
		return;
	}
	device->setName( d->nameLineEdit->text().trimmed() );
	device->setProtocol("group");
	QString shownIds = addedProxyModel->getShownIds();
	device->setParameter("devices", shownIds);
	this->accept();
}

void EditGroupDialog::availableListActivated(const QModelIndex &) {
	addToolButton->setEnabled(true);
}

void EditGroupDialog::addedListActivated(const QModelIndex &) {
	removeToolButton->setEnabled(true);
}

ProxyModel::ProxyModel( QObject * parent )
		: QSortFilterProxyModel(parent)
{
}

ProxyModel::~ProxyModel() {
}

void ProxyModel::showAllRows() {
	for( int i = 0; i < this->sourceModel()->rowCount(); ++i ) {
		rows.insert(i);
	}
}

QString ProxyModel::getShownIds(){
	QStringList addedIds;
	foreach (int index, rows){	//TODO why does this crash when debugging? Unsafe?
		if(index >= 0){	//TODO why can this be 
			DeviceModel *model = reinterpret_cast<DeviceModel *>(this->sourceModel());
			if (model) {
				int deviceId = model->deviceId(model->index(index, 0));
				addedIds << QString::number(deviceId);
			}
		}
	}

	return addedIds.join(",");
}

void ProxyModel::hideRow( int row ) {
	if (rows.contains(row)) {
		rows.remove(row);
		invalidateFilter();
	}
}

void ProxyModel::showRow( int row ) {
	
	if (!rows.contains(row)) {
		rows.insert(row);
		invalidateFilter();
	}
}

int ProxyModel::columnCount ( const QModelIndex & ) const {
	return 2;
}

bool ProxyModel::filterAcceptsRow ( int source_row, const QModelIndex & ) const {
	return rows.contains(source_row);
}
