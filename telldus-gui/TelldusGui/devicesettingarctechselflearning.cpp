//
// C++ Implementation: devicesettingarctechselflearning
//
// Description:
//
//
// Author: Fredrik Jacobsson <fredrik.jacobsson@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "devicesettingarctechselflearning.h"
#include "device.h"
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <time.h>

DeviceSettingArctechSelflearning::DeviceSettingArctechSelflearning(Device *device, QWidget *parent)
 : DeviceSetting(device, parent)
{
	QGridLayout *gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(6);
	gridLayout->setMargin(9);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

	gridLayout->addItem( new QSpacerItem(20, 109, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 0 );

	QLabel *labelRemotecodeTitle = new QLabel(this);
	labelRemotecodeTitle->setObjectName(QString::fromUtf8("labelRemotecodeTitle"));
	labelRemotecodeTitle->setAlignment(Qt::AlignCenter);
	labelRemotecodeTitle->setText( tr("Remote code") );
	gridLayout->addWidget(labelRemotecodeTitle, 1, 0);

	QLabel *labelUnitcodeTitle = new QLabel(this);
	labelUnitcodeTitle->setObjectName(QString::fromUtf8("labelUnitcodeTitle"));
	labelUnitcodeTitle->setAlignment(Qt::AlignCenter);
	labelUnitcodeTitle->setText( tr("Unitcode") );
	gridLayout->addWidget(labelUnitcodeTitle, 1, 1);

	spinRemotecode = new QSpinBox(this);
	spinRemotecode->setObjectName(QString::fromUtf8("unitcode"));
	spinRemotecode->setMinimum(1);
	spinRemotecode->setMaximum(67108863);
	gridLayout->addWidget(spinRemotecode, 2, 0);

	spinUnitcode = new QSpinBox(this);
	spinUnitcode->setObjectName(QString::fromUtf8("unitcode"));
	spinUnitcode->setMinimum(1);
	spinUnitcode->setMaximum(16);
	gridLayout->addWidget(spinUnitcode, 2, 1);

	gridLayout->addItem( new QSpacerItem(20, 109, QSizePolicy::Minimum, QSizePolicy::Expanding), 3, 0 );

 	spinRemotecode->setValue( device->parameter("house", "1").toInt() );
 	spinUnitcode->setValue( device->parameter("unit", "1").toInt() );

	QPushButton *randomButton = new QPushButton( tr("Randomize"), this);
	connect(randomButton, SIGNAL(clicked()), this, SLOT(randomizeCode()));
	gridLayout->addWidget( randomButton, 3, 0 );
	
	//Seed the random number generator at widget creation
	srand( (unsigned int)time( NULL ) );

}


DeviceSettingArctechSelflearning::~DeviceSettingArctechSelflearning()
{
}

void DeviceSettingArctechSelflearning::saveParameters() {
 	p_device->setParameter( "house", QString::number(spinRemotecode->value()) );
 	p_device->setParameter( "unit", QString::number(spinUnitcode->value()) );

}


void DeviceSettingArctechSelflearning::setRemoteMinMax(int min, int max) {
	spinRemotecode->setMinimum(min);
	spinRemotecode->setMaximum(max);
}

void DeviceSettingArctechSelflearning::setUnitMinMax(int min, int max) {
	spinUnitcode->setMinimum(min);
	spinUnitcode->setMaximum(max);
}

void DeviceSettingArctechSelflearning::randomizeCode() {
	int randomNumber = rand() % spinRemotecode->maximum() + spinRemotecode->minimum(); //Generate ranom number between min and max

	spinRemotecode->setValue(randomNumber);
}
