#include "editdevicedialog.h"
#include "filtereddeviceproxymodel.h"
#include "vendordevicemodel.h"
#include "vendordevicetreeitem.h"
#include "device.h"

#include "devicesettingarctechselflearning.h"
#include "devicesettingbrateck.h"
#include "devicesettinggao.h"
#include "devicesettingikea.h"
#include "devicesettingnexa.h"
#include "devicesettingnexabell.h"
#include "devicesettingrisingsun.h"
#include "devicesettingsartano.h"
#include "devicesettingupm.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QFormLayout>
#include <QPushButton>
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
		filteredModel(new FilteredDeviceProxyModel(this)),
		device(d),
		settingsLayout(0),
		deviceImage(0),
		nameLineEdit(0),
		selection(0),
		scanning(false)
{
	filteredModel->setSourceModel(model);

	QVBoxLayout *layout = new QVBoxLayout(this);
#ifdef Q_WS_HILDON
	stacked_layout = new QStackedLayout;
	layout->addLayout(stacked_layout);
#endif

	QHBoxLayout *scanLayout = new QHBoxLayout;
	QLabel *scanLabel = new QLabel(tr("TellStick Duo found.<br>Press scan button to search for devices"), this);
	scanLayout->addWidget(scanLabel);
	scanLayout->addStretch();
	scanButton = new QPushButton( tr("Scan"), this);
	connect(scanButton, SIGNAL(clicked()), this, SLOT(scanClicked()));
	scanLayout->addWidget(scanButton);
	stopScanButton = new QPushButton( tr("Stop"), this);
	stopScanButton->setEnabled(false);
	connect(stopScanButton, SIGNAL(clicked()), this, SLOT(stopScanClicked()));
	scanLayout->addWidget(stopScanButton);

	QGroupBox *scanGroupBox = new QGroupBox(this);
	scanGroupBox->setTitle( tr("Scan") );
	scanGroupBox->setLayout(scanLayout);
	layout->addWidget(scanGroupBox);

	QHBoxLayout *deviceLayout = new QHBoxLayout;

	QTreeView *deviceView = new QTreeView(this);
	deviceView->setModel( filteredModel );
	deviceView->setMinimumSize( QSize(200, 200) );

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
	deviceSettings[5] = new DeviceSettingRisingSun(device, this);
	deviceSettings[6] = new DeviceSettingBrateck(device, this);
	deviceSettings[7] = new DeviceSettingUpm(device, this); //Not used?
	deviceSettings[8] = new DeviceSettingArctechSelflearning(device, this);
	deviceSettings[9] = new DeviceSettingArctechSelflearning(device, this);
	((DeviceSettingArctechSelflearning *)deviceSettings[9])->setRemoteMinMax(0,4095);
	((DeviceSettingArctechSelflearning *)deviceSettings[9])->setUnitMinMax(1,4);
	deviceSettings[10] = new DeviceSettingGAO(device, this);
	deviceSettings[11] = new DeviceSettingArctechSelflearning(device, this);
	((DeviceSettingArctechSelflearning *)deviceSettings[11])->setRemoteMinMax(0,16383);
	((DeviceSettingArctechSelflearning *)deviceSettings[11])->setUnitMinMax(1,4);
	deviceSettings[12] = new DeviceSettingArctechSelflearning(device, this);
	((DeviceSettingArctechSelflearning *)deviceSettings[12])->setRemoteMinMax(1,33554432);
	((DeviceSettingArctechSelflearning *)deviceSettings[12])->setUnitMinMax(1,16);

	
	foreach( DeviceSetting *s, deviceSettings ) {
		connect(filteredModel, SIGNAL(setParameter(const QString&, const QString&)), s, SLOT(setValue(const QString&, const QString&)));
		settingsLayout->addWidget( s );
	}

	expandNodes(deviceView);
	QModelIndex index = filteredModel->mapFromSource(model->index( device ));
	if (index.isValid()) {
		deviceView->expand( index.parent() );
		selection->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect );
	}

	connect(this, SIGNAL(rawDataReceived(const QString &)), this, SLOT(rawDataSlot(const QString &)));
	rawDeviceEventId = tdRegisterRawDeviceEvent(reinterpret_cast<TDRawDeviceEvent>(&EditDeviceDialog::rawData), this);
}

EditDeviceDialog::~EditDeviceDialog() {
	tdUnregisterCallback(rawDeviceEventId);
//	qDeleteAll( deviceSettings );
}

void EditDeviceDialog::selectionChanged( const QModelIndex & filteredIndex ) {
	QModelIndex index = filteredModel->mapToSource( filteredIndex );

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

void EditDeviceDialog::scanClicked() {
	scanButton->setEnabled( false );
	stopScanButton->setEnabled( true );
	scanning = true;
}

void EditDeviceDialog::stopScanClicked() {
	scanButton->setEnabled( true );
	stopScanButton->setEnabled( false );
	scanning = false;
	filteredModel->showAll();
}

void EditDeviceDialog::okClicked() {

	VendorDeviceTreeItem* const item = model->item( filteredModel->mapToSource(selection->currentIndex()) );
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

void EditDeviceDialog::expandNodes(QTreeView *deviceView) {
	for( int i = 0; i < model->rowCount(QModelIndex()); ++i ) {
		QModelIndex index = model->index(i, 0, QModelIndex());
		VendorDeviceTreeItem *item = model->item(index);
		if (item && item->isExpanded()) {
			deviceView->expand(filteredModel->mapFromSource(index));
		}
	}
}

void EditDeviceDialog::rawDataSlot( const QString &data ) {
	if (!scanning) {
		return;
	}
	filteredModel->setFilter(data);
}

void WINAPI EditDeviceDialog::rawData(const char *data, int callbackId, void *context) {
	EditDeviceDialog *dialog = reinterpret_cast<EditDeviceDialog *>(context);
	if (dialog) {
		emit dialog->rawDataReceived(data);
	}
}
