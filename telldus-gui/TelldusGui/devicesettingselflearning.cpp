//
// C++ Implementation: devicesettingselflearning
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "devicesettingselflearning.h"
#include "device.h"
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <time.h>

DeviceSettingSelflearning::DeviceSettingSelflearning(Device *device, QWidget *parent)
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

	spinRemotecode = new QSpinBox(this);
	spinRemotecode->setObjectName(QString::fromUtf8("unitcode"));
	spinRemotecode->setMinimum(1);
	spinRemotecode->setMaximum(67108863);
	gridLayout->addWidget(spinRemotecode, 2, 0);

	gridLayout->addItem( new QSpacerItem(20, 109, QSizePolicy::Minimum, QSizePolicy::Expanding), 3, 0 );

	spinRemotecode->setValue( device->parameter("house", "1").toInt() );

	QPushButton *randomButton = new QPushButton( tr("Randomize"), this);
	connect(randomButton, SIGNAL(clicked()), this, SLOT(randomizeCode()));
	gridLayout->addWidget( randomButton, 3, 0 );

	//Seed the random number generator at widget creation
	srand( (unsigned int)time( NULL ) );

}


DeviceSettingSelflearning::~DeviceSettingSelflearning()
{
}

void DeviceSettingSelflearning::saveParameters() {
	p_device->setParameter( "house", QString::number(spinRemotecode->value()) );
}

void DeviceSettingSelflearning::setValue( const QString &name, const QString &value ) {
	if (name == "house") {
		spinRemotecode->setValue(value.toInt());
	}
}


void DeviceSettingSelflearning::setRemoteMinMax(int min, int max) {
	spinRemotecode->setMinimum(min);
	spinRemotecode->setMaximum(max);
}

void DeviceSettingSelflearning::randomizeCode() {
	int randomNumber = rand() % spinRemotecode->maximum() + spinRemotecode->minimum(); //Generate ranom number between min and max

	spinRemotecode->setValue(randomNumber);
}
