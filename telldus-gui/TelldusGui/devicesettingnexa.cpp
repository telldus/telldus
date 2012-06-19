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
#include "devicesettingnexa.h"
#include "device.h"
#include <QGridLayout>
#include <QDial>
#include <QLabel>

DeviceSettingNexa::DeviceSettingNexa(Device *device, QWidget *parent)
 : DeviceSetting(device, parent),
	dialHouse(0),
	dialUnit(0),
	labelHouse(0),
	labelUnit(0)
{
	QGridLayout *gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(6);
	gridLayout->setMargin(9);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

	QLabel *labelHouseTitle = new QLabel(this);
	labelHouseTitle->setObjectName(QString::fromUtf8("labelHouseTitle"));
	labelHouseTitle->setAlignment(Qt::AlignCenter);
	labelHouseTitle->setText( tr("Housecode") );
	gridLayout->addWidget(labelHouseTitle, 0, 0, 1, 1);

	QLabel *labelUnitTitle = new QLabel(this);
	labelUnitTitle->setObjectName(QString::fromUtf8("labelUnitTitle"));
	labelUnitTitle->setAlignment(Qt::AlignCenter);
	labelUnitTitle->setText( tr("Unitcode") );
	gridLayout->addWidget(labelUnitTitle, 0, 1, 1, 1);

	dialHouse = new QDial(this);
	dialHouse->setObjectName(QString::fromUtf8("dialHouse"));
	dialHouse->setMinimum(0);
	dialHouse->setMaximum(15);
	dialHouse->setPageStep(1);
	dialHouse->setOrientation(Qt::Horizontal);
	dialHouse->setNotchesVisible(true);
	gridLayout->addWidget(dialHouse, 1, 0, 1, 1);

	dialUnit = new QDial(this);
	dialUnit->setObjectName(QString::fromUtf8("dialCode"));
	dialUnit->setMinimum(1);
	dialUnit->setMaximum(16);
	dialUnit->setPageStep(1);
	dialUnit->setOrientation(Qt::Horizontal);
	dialUnit->setNotchesVisible(true);
	gridLayout->addWidget(dialUnit, 1, 1, 1, 1);

	labelHouse = new QLabel(this);
	labelHouse->setObjectName(QString::fromUtf8("labelHouse"));
	labelHouse->setAlignment(Qt::AlignCenter);
	labelHouse->setText( "A" );
	gridLayout->addWidget(labelHouse, 2, 0, 1, 1);

	labelUnit = new QLabel(this);
	labelUnit->setObjectName(QString::fromUtf8("labelUnit"));
	labelUnit->setAlignment(Qt::AlignCenter);
	labelUnit->setText( "1" );
	gridLayout->addWidget(labelUnit, 2, 1, 1, 1);

	connect( dialHouse, SIGNAL( valueChanged( int ) ), this, SLOT( houseChanged(int) ) );
	connect( dialUnit, SIGNAL(valueChanged(int)), labelUnit, SLOT(setNum(int)) );

	this->setValue( "house", device->parameter("house", "A"));
	this->setValue( "unit", device->parameter("unit", "1") );
}


DeviceSettingNexa::~DeviceSettingNexa()
{
}

void DeviceSettingNexa::saveParameters() {
	p_device->setParameter( "house", QString('A' + dialHouse->value()) );
	p_device->setParameter( "unit", QString::number(dialUnit->value()) );
}

void DeviceSettingNexa::setValue( const QString &name, const QString &value ) {
	if (name == "house") {
		uint intHouse = 0;
		if (value.length() > 0) {
			intHouse = value[0].toUpper().toAscii() - 'A';
		}
		dialHouse->setValue( intHouse );
	} else if (name == "unit") {
		dialUnit->setValue( value.toInt() );
	}
}

void DeviceSettingNexa::houseChanged( int house )
{
	labelHouse->setText( QString('A' + house) );
}

