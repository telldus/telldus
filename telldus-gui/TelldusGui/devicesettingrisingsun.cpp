//
// C++ Implementation: devicesettingnexa
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "devicesettingrisingsun.h"
#include "device.h"
#include <QGridLayout>
#include <QDial>
#include <QLabel>

DeviceSettingRisingSun::DeviceSettingRisingSun(Device *device, QWidget *parent)
 : DeviceSetting(device, parent),
	dialHouse(0),
	dialUnit(0)
{
	QVBoxLayout *verticalLayout = new QVBoxLayout(this);
	verticalLayout->addSpacerItem( new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding) );

	QHBoxLayout *horizontalLayout = new QHBoxLayout;
	verticalLayout->addLayout(horizontalLayout);
	verticalLayout->addSpacerItem( new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding) );

	QGridLayout *gridLayoutLeft = new QGridLayout;
	horizontalLayout->addSpacerItem( new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum) );
	horizontalLayout->addLayout( gridLayoutLeft );

	horizontalLayout->addSpacerItem( new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum) );

	QGridLayout *gridLayoutRight = new QGridLayout;
	horizontalLayout->addLayout( gridLayoutRight );
	horizontalLayout->addSpacerItem( new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum) );

	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);

	QLabel *houseNotchLabel = new QLabel("I", this);
	houseNotchLabel->setSizePolicy(sizePolicy);
	houseNotchLabel->setAlignment(Qt::AlignCenter);
	gridLayoutLeft->addWidget(houseNotchLabel, 1, 0, 2, 1);

	houseNotchLabel = new QLabel("II", this);
	houseNotchLabel->setSizePolicy(sizePolicy);
	houseNotchLabel->setAlignment(Qt::AlignCenter);
	gridLayoutLeft->addWidget(houseNotchLabel, 0, 1, 1, 2);

	houseNotchLabel = new QLabel("III", this);
	houseNotchLabel->setSizePolicy(sizePolicy);
	houseNotchLabel->setAlignment(Qt::AlignCenter);
	gridLayoutLeft->addWidget(houseNotchLabel, 1, 3, 2, 1);

	houseNotchLabel = new QLabel("IV", this);
	houseNotchLabel->setSizePolicy(sizePolicy);
	houseNotchLabel->setAlignment(Qt::AlignCenter);
	gridLayoutLeft->addWidget(houseNotchLabel, 3, 1, 1, 2);

	dialHouse = new QDial(this);
	dialHouse->setObjectName(QString::fromUtf8("dialHouse"));
	dialHouse->setSizePolicy(sizePolicy);
	dialHouse->setMinimum(0);
	dialHouse->setMaximum(4);
	dialHouse->setPageStep(1);
	dialHouse->setOrientation(Qt::Horizontal);
	dialHouse->setNotchesVisible(true);
	dialHouse->setWrapping(true);
	gridLayoutLeft->addWidget(dialHouse, 1, 1, 1, 2);

	houseNotchLabel = new QLabel("1", this);
	houseNotchLabel->setSizePolicy(sizePolicy);
	houseNotchLabel->setAlignment(Qt::AlignCenter);
	gridLayoutRight->addWidget(houseNotchLabel, 1, 0, 2, 1);

	houseNotchLabel = new QLabel("2", this);
	houseNotchLabel->setSizePolicy(sizePolicy);
	houseNotchLabel->setAlignment(Qt::AlignCenter);
	gridLayoutRight->addWidget(houseNotchLabel, 0, 1, 1, 2);

	houseNotchLabel = new QLabel("3", this);
	houseNotchLabel->setSizePolicy(sizePolicy);
	houseNotchLabel->setAlignment(Qt::AlignCenter);
	gridLayoutRight->addWidget(houseNotchLabel, 1, 3, 2, 1);

	houseNotchLabel = new QLabel("4", this);
	houseNotchLabel->setSizePolicy(sizePolicy);
	houseNotchLabel->setAlignment(Qt::AlignCenter);
	gridLayoutRight->addWidget(houseNotchLabel, 3, 1, 1, 2);


	dialUnit = new QDial(this);
	dialUnit->setObjectName(QString::fromUtf8("dialHouse"));
	dialUnit->setSizePolicy(sizePolicy);
	dialUnit->setMinimum(0);
	dialUnit->setMaximum(4);
	dialUnit->setPageStep(1);
	dialUnit->setOrientation(Qt::Horizontal);
	dialUnit->setNotchesVisible(true);
	dialUnit->setWrapping(true);
	gridLayoutRight->addWidget(dialUnit, 1, 1, 1, 2);

	dialHouse->setValue( device->parameter("nexa_house", "1").toInt() );
	dialUnit->setValue( device->parameter("nexa_unit", "1").toInt() );
}


DeviceSettingRisingSun::~DeviceSettingRisingSun()
{
}

void DeviceSettingRisingSun::saveParameters() {
	int house = (dialHouse->value() == 0 ? 4 : dialHouse->value());
	int unit = (dialUnit->value() == 0 ? 4 : dialUnit->value());
	p_device->setParameter( "nexa_house", QString::number(house) );
	p_device->setParameter( "nexa_unit", QString::number(unit) );
}

