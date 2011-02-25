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
#include "devicesettingunitcode.h"
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

class EditDeviceDialog::PrivateData {
public:
	VendorDeviceModel *model;
	FilteredDeviceProxyModel *filteredModel;
	Device *device;
	QStackedLayout *settingsLayout;
	QLabel *deviceImage, *scanLabel;
	QPushButton *scanButton, *stopScanButton;
	QLineEdit *nameLineEdit;
	QItemSelectionModel *selection;
	QHash<int, DeviceSetting *> deviceSettings;
	int rawDeviceEventId;
	bool scanning;
#ifdef Q_WS_HILDON
	QStackedLayout *stacked_layout;
#endif
};

EditDeviceDialog::EditDeviceDialog(Device *device, QWidget *parent, Qt::WFlags flags)
		:QDialog(parent, flags),
		d(new PrivateData)

{
	d->model = new VendorDeviceModel(this);
	d->filteredModel = new FilteredDeviceProxyModel(this);
	d->filteredModel->setSourceModel(d->model);
	d->device = device;
	d->scanning = false;

	QVBoxLayout *layout = new QVBoxLayout(this);
#ifdef Q_WS_HILDON
	stacked_layout = new QStackedLayout;
	layout->addLayout(stacked_layout);
#endif

	QHBoxLayout *scanLayout = new QHBoxLayout;
	d->scanLabel = new QLabel(this);
	scanLayout->addWidget(d->scanLabel);
	scanLayout->addStretch();
	d->scanButton = new QPushButton( tr("Scan"), this);
	connect(d->scanButton, SIGNAL(clicked()), this, SLOT(scanClicked()));
	scanLayout->addWidget(d->scanButton);
	d->stopScanButton = new QPushButton( tr("Stop"), this);
	d->stopScanButton->setEnabled(false);
	connect(d->stopScanButton, SIGNAL(clicked()), this, SLOT(stopScanClicked()));
	scanLayout->addWidget(d->stopScanButton);

	QGroupBox *scanGroupBox = new QGroupBox(this);
	scanGroupBox->setTitle( tr("Scan") );
	scanGroupBox->setLayout(scanLayout);
	layout->addWidget(scanGroupBox);

	QHBoxLayout *deviceLayout = new QHBoxLayout;

	QTreeView *deviceView = new QTreeView(this);
	deviceView->setModel( d->filteredModel );
	deviceView->setMinimumSize( QSize(200, 200) );

	d->selection = deviceView->selectionModel();
	connect( d->selection, SIGNAL( currentChanged(const QModelIndex, const QModelIndex &) ), this, SLOT(selectionChanged( const QModelIndex & ) ));
	deviceLayout->addWidget(deviceView);

	QVBoxLayout *deviceInfoLayout = new QVBoxLayout;
	d->deviceImage = new QLabel( this );
	deviceInfoLayout->addWidget( d->deviceImage );
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
	d->nameLineEdit = new QLineEdit(device->name(),  this );
	nameLabel->setBuddy(d->nameLineEdit);

	nameLayout->addRow(nameLabel, d->nameLineEdit);
	addressLayout->addLayout( nameLayout );

	d->settingsLayout = new QStackedLayout;
	QLabel *noDeviceLabel = new QLabel( tr("Choose a device above"), this );
	noDeviceLabel->setAlignment( Qt::AlignCenter );
	d->settingsLayout->addWidget( noDeviceLabel );
	addressLayout->addLayout(d->settingsLayout);

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

	d->deviceSettings[1] = new DeviceSettingNexa(device, this);
	d->deviceSettings[2] = new DeviceSettingSartano(device, this);
	d->deviceSettings[3] = new DeviceSettingIkea(device, this);
	d->deviceSettings[4] = new DeviceSettingNexaBell(device, this);
	d->deviceSettings[5] = new DeviceSettingRisingSun(device, this);
	d->deviceSettings[6] = new DeviceSettingBrateck(device, this);
	d->deviceSettings[7] = new DeviceSettingUpm(device, this); //Not used?
	d->deviceSettings[8] = new DeviceSettingArctechSelflearning(device, this);
	d->deviceSettings[9] = new DeviceSettingArctechSelflearning(device, this);
	((DeviceSettingArctechSelflearning *)d->deviceSettings[9])->setRemoteMinMax(0,4095);
	((DeviceSettingArctechSelflearning *)d->deviceSettings[9])->setUnitMinMax(1,4);
	d->deviceSettings[10] = new DeviceSettingGAO(device, this);
	d->deviceSettings[11] = new DeviceSettingArctechSelflearning(device, this);
	((DeviceSettingArctechSelflearning *)d->deviceSettings[11])->setRemoteMinMax(0,16383);
	((DeviceSettingArctechSelflearning *)d->deviceSettings[11])->setUnitMinMax(1,4);
	d->deviceSettings[12] = new DeviceSettingArctechSelflearning(device, this);
	((DeviceSettingArctechSelflearning *)d->deviceSettings[12])->setRemoteMinMax(1,33554432);
	((DeviceSettingArctechSelflearning *)d->deviceSettings[12])->setUnitMinMax(1,16);
	d->deviceSettings[13] = new DeviceSettingUnitcode(device, this);
	//Ecosavers
	d->deviceSettings[14] = new DeviceSettingArctechSelflearning(device, this);
	((DeviceSettingArctechSelflearning *)d->deviceSettings[14])->setRemoteMinMax(1,1048575);
	((DeviceSettingArctechSelflearning *)d->deviceSettings[14])->setUnitMinMax(1,4);

	foreach( DeviceSetting *s, d->deviceSettings ) {
		connect(d->filteredModel, SIGNAL(setParameter(const QString&, const QString&)), s, SLOT(setValue(const QString&, const QString&)));
		d->settingsLayout->addWidget( s );
	}

	expandNodes(deviceView);
	QModelIndex index = d->filteredModel->mapFromSource(d->model->index( device ));
	if (index.isValid()) {
		deviceView->expand( index.parent() );
		d->selection->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect );
	}

	connect(this, SIGNAL(rawDataReceived(const QString &)), this, SLOT(rawDataSlot(const QString &)), Qt::QueuedConnection);
	d->rawDeviceEventId = tdRegisterRawDeviceEvent(reinterpret_cast<TDRawDeviceEvent>(&EditDeviceDialog::rawData), this);

	stopScanClicked();
}

EditDeviceDialog::~EditDeviceDialog() {
	tdUnregisterCallback(d->rawDeviceEventId);
	delete d;
}

void EditDeviceDialog::selectionChanged( const QModelIndex & filteredIndex ) {
	QModelIndex index = d->filteredModel->mapToSource( filteredIndex );

	VendorDeviceTreeItem* const item = d->model->item(index);
	if (!item) {
		return;
	}

	d->deviceImage->setPixmap( item->image() );

	int widget = item->widget();
	if (widget >= d->settingsLayout->count()) {
		widget = 0;
	}
	d->settingsLayout->setCurrentIndex( widget );
}

void EditDeviceDialog::scanClicked() {
	d->scanLabel->setText(tr("Please press a button on your remote"));
	d->scanButton->setEnabled( false );
	d->stopScanButton->setEnabled( true );
	d->scanning = true;
}

void EditDeviceDialog::stopScanClicked() {
	d->scanLabel->setText(tr("If you have a TellStick Duo connected,<br>you can use it to scan the code of the remote control"));
	d->scanButton->setEnabled( true );
	d->stopScanButton->setEnabled( false );
	d->scanning = false;
	d->filteredModel->showAll();
}

void EditDeviceDialog::okClicked() {

	VendorDeviceTreeItem* const item = d->model->item( d->filteredModel->mapToSource(d->selection->currentIndex()) );
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

	d->device->setName( d->nameLineEdit->text().trimmed() );
	d->device->setModel( item->deviceModel() );
	d->device->setProtocol( item->deviceProtocol() );

	DeviceSetting *w = qobject_cast<DeviceSetting *>(d->settingsLayout->currentWidget());
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
	for( int i = 0; i < d->model->rowCount(QModelIndex()); ++i ) {
		QModelIndex index = d->model->index(i, 0, QModelIndex());
		VendorDeviceTreeItem *item = d->model->item(index);
		if (item && item->isExpanded()) {
			deviceView->expand(d->filteredModel->mapFromSource(index));
		}
	}
}

void EditDeviceDialog::rawDataSlot( const QString &data ) {
	if (!d->scanning) {
		return;
	}
	d->filteredModel->addFilter(data);
}

void WINAPI EditDeviceDialog::rawData(const char *data, int controllerId, int callbackId, void *context) {
	EditDeviceDialog *dialog = reinterpret_cast<EditDeviceDialog *>(context);
	if (dialog) {
		emit dialog->rawDataReceived(data);
	}
}
