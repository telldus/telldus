#include "editdevicedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QFormLayout>
#include <QTreeView>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

EditDeviceDialog::EditDeviceDialog(QWidget *parent, Qt::WFlags flags)
		:QDialog(parent, flags)
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	QHBoxLayout *deviceLayout = new QHBoxLayout(this);

	QTreeView *deviceView = new QTreeView(this);
	deviceLayout->addWidget(deviceView);

	QGroupBox *deviceGroupBox = new QGroupBox(this);
	deviceGroupBox->setTitle( tr("Device") );
	deviceGroupBox->setLayout(deviceLayout);
	layout->addWidget(deviceGroupBox);

	QVBoxLayout *addressLayout = new QVBoxLayout(this);

	QFormLayout *nameLayout = new QFormLayout(this);

	QLabel *nameLabel = new QLabel(this);
	nameLabel->setText( tr("&Name:") );
	QLineEdit *nameLineEdit = new QLineEdit( this );
	nameLabel->setBuddy(nameLineEdit);

	nameLayout->addRow(nameLabel, nameLineEdit);
	addressLayout->addLayout( nameLayout );

	QStackedLayout *settingsLayout = new QStackedLayout(this);
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
}

EditDeviceDialog::~EditDeviceDialog() {
}
