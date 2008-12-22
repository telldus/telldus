#include "editdevicedialog.h"
#include "vendordevicemodel.h"
#include "vendordevicetreeitem.h"

#include "devicesettingnexa.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QFormLayout>
#include <QTreeView>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

#include <QDebug>

EditDeviceDialog::EditDeviceDialog(QWidget *parent, Qt::WFlags flags)
		:QDialog(parent, flags),
		model(new VendorDeviceModel(this))
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	QHBoxLayout *deviceLayout = new QHBoxLayout;

	QTreeView *deviceView = new QTreeView(this);
	deviceView->setModel( model );
	QItemSelectionModel *selection = deviceView->selectionModel();
	connect( selection, SIGNAL( currentChanged(const QModelIndex, const QModelIndex &) ), this, SLOT(selectionChanged( const QModelIndex & ) ));
	deviceLayout->addWidget(deviceView);

	QVBoxLayout *deviceInfoLayout = new QVBoxLayout;
	deviceImage = new QLabel( this );
	deviceInfoLayout->addWidget( deviceImage );
	deviceInfoLayout->addStretch();

	deviceLayout->addLayout( deviceInfoLayout );

	QGroupBox *deviceGroupBox = new QGroupBox(this);
	deviceGroupBox->setTitle( tr("Device") );
	deviceGroupBox->setLayout(deviceLayout);
	layout->addWidget(deviceGroupBox);

	QVBoxLayout *addressLayout = new QVBoxLayout;

	QFormLayout *nameLayout = new QFormLayout;

	QLabel *nameLabel = new QLabel(this);
	nameLabel->setText( tr("&Name:") );
	QLineEdit *nameLineEdit = new QLineEdit( this );
	nameLabel->setBuddy(nameLineEdit);

	nameLayout->addRow(nameLabel, nameLineEdit);
	addressLayout->addLayout( nameLayout );

	settingsLayout = new QStackedLayout;
	QLabel *noDeviceLabel = new QLabel( tr("Choose a device above"), this );
	noDeviceLabel->setAlignment( Qt::AlignCenter );
	settingsLayout->addWidget( noDeviceLabel );
	addressLayout->addLayout(settingsLayout);

	QGroupBox *settingsGroupBox = new QGroupBox(this);
	settingsGroupBox->setTitle( tr("Addresscode") );
	settingsGroupBox->setLayout( addressLayout );
	layout->addWidget( settingsGroupBox );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
	buttonBox->setStandardButtons( QDialogButtonBox::Save | QDialogButtonBox::Cancel );
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	layout->addWidget(buttonBox);

	deviceSettings[1] = new DeviceSettingNexa();
	foreach( DeviceSetting *s, deviceSettings ) {
		settingsLayout->addWidget( s );
	}
}

EditDeviceDialog::~EditDeviceDialog() {
	qDeleteAll( deviceSettings );
}

void EditDeviceDialog::selectionChanged( const QModelIndex & index ) {
	const VendorDeviceTreeItem* const item = model->item(index);
	if (!item) {
		return;
	}

	deviceImage->setPixmap( item->image() );
	settingsLayout->setCurrentIndex( item->widget() );
}
