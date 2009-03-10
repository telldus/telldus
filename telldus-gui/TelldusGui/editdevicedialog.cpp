#include "editdevicedialog.h"
#include "vendordevicemodel.h"
#include "vendordevicetreeitem.h"
#include "device.h"

#include "devicesettingikea.h"
#include "devicesettingnexa.h"
#include "devicesettingnexabell.h"
#include "devicesettingsartano.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QFormLayout>
#include <QTreeView>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QMessageBox>

#include <QDebug>

EditDeviceDialog::EditDeviceDialog(Device *d, QWidget *parent, Qt::WFlags flags)
		:QDialog(parent, flags),
		model(new VendorDeviceModel(this)),
		device(d),
		settingsLayout(0),
		deviceImage(0),
		nameLineEdit(0),
		selection(0)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
#ifdef Q_WS_HILDON
	stacked_layout = new QStackedLayout;
	layout->addLayout(stacked_layout);
#endif

	QHBoxLayout *deviceLayout = new QHBoxLayout;

	QTreeView *deviceView = new QTreeView(this);
	deviceView->setModel( model );

	selection = deviceView->selectionModel();
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
#ifdef Q_WS_HILDON
	stacked_layout->addWidget(deviceGroupBox);
#else
	layout->addWidget(deviceGroupBox);
#endif

	QVBoxLayout *addressLayout = new QVBoxLayout;

	QFormLayout *nameLayout = new QFormLayout;

	QLabel *nameLabel = new QLabel(this);
	nameLabel->setText( tr("&Name:") );
	nameLineEdit = new QLineEdit(device->name(),  this );
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
#ifdef Q_WS_HILDON
	stacked_layout->addWidget(settingsGroupBox);
#else
	layout->addWidget( settingsGroupBox );
#endif

	QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
	buttonBox->setStandardButtons( QDialogButtonBox::Save | QDialogButtonBox::Cancel );
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(okClicked()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancelClicked()));
	layout->addWidget(buttonBox);

	deviceSettings[1] = new DeviceSettingNexa(device, this);
	deviceSettings[2] = new DeviceSettingSartano(device, this);
	deviceSettings[3] = new DeviceSettingIkea(device, this);
	deviceSettings[4] = new DeviceSettingNexaBell(device, this);
	foreach( DeviceSetting *s, deviceSettings ) {
		settingsLayout->addWidget( s );
	}

	QModelIndex index = model->index( device );
	if (index.isValid()) {
		deviceView->expand( index.parent() );
		selection->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect );
	}

}

EditDeviceDialog::~EditDeviceDialog() {
	qDeleteAll( deviceSettings );
}

void EditDeviceDialog::selectionChanged( const QModelIndex & index ) {
	VendorDeviceTreeItem* const item = model->item(index);
	if (!item) {
		return;
	}

	deviceImage->setPixmap( item->image() );

	int widget = item->widget();
	if (widget >= settingsLayout->count()) {
		widget = 0;
	}
	settingsLayout->setCurrentIndex( widget );
}

void EditDeviceDialog::okClicked() {

	VendorDeviceTreeItem* const item = model->item( selection->currentIndex() );
	if (!item || !item->isDevice()) {
		QMessageBox msgBox;
		msgBox.setText( tr("You must choose a device") );
		msgBox.setInformativeText( tr("Please select the device you have.") );
		msgBox.setIcon( QMessageBox::Critical );
		msgBox.setStandardButtons( QMessageBox::Ok );
		msgBox.exec();
		return;
	}

#ifdef Q_WS_HILDON
	if (stacked_layout->currentIndex() == 0) {
		stacked_layout->setCurrentIndex(1);
		return;
	}
#endif

	if (nameLineEdit->text().trimmed() == "") {
		QMessageBox msgBox;
		msgBox.setText( tr("The device must have a name.") );
		msgBox.setInformativeText( tr("Please fill in a name in the field under 'Name'") );
		msgBox.setIcon( QMessageBox::Critical );
		msgBox.setStandardButtons( QMessageBox::Ok );
		msgBox.exec();
		nameLineEdit->setFocus();
		return;
	}

	device->setName( nameLineEdit->text().trimmed() );
	device->setModel( item->deviceModel() );
	device->setProtocol( item->deviceProtocol() );

	DeviceSetting *w = qobject_cast<DeviceSetting *>(settingsLayout->currentWidget());
	if (w) {
		w->saveParameters();
	}

	this->accept();
}

void EditDeviceDialog::cancelClicked() {
#ifdef Q_WS_HILDON
	if (stacked_layout->currentIndex() > 0) {
		stacked_layout->setCurrentIndex(stacked_layout->currentIndex()-1);
		return;
	}
#endif
	this->reject();
}
