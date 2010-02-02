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
#include "devicesettingnexabell.h"
#include "device.h"
#include <QGridLayout>
#include <QDial>
#include <QLabel>

DeviceSettingNexaBell::DeviceSettingNexaBell(Device *device, QWidget *parent)
 : DeviceSetting(device, parent)
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

	dialHouse = new QDial(this);
	dialHouse->setObjectName(QString::fromUtf8("dialHouse"));
	dialHouse->setMinimum(0);
	dialHouse->setMaximum(15);
	dialHouse->setPageStep(1);
	dialHouse->setOrientation(Qt::Horizontal);
	dialHouse->setNotchesVisible(true);
	gridLayout->addWidget(dialHouse, 1, 0, 1, 1);

	labelHouse = new QLabel(this);
	labelHouse->setObjectName(QString::fromUtf8("labelHouse"));
	labelHouse->setAlignment(Qt::AlignCenter);
	labelHouse->setText( "A" );
	gridLayout->addWidget(labelHouse, 2, 0, 1, 1);

	connect( dialHouse, SIGNAL( valueChanged( int ) ), this, SLOT( houseChanged(int) ) );

	QString strHouse = device->parameter("house", "A");
	uint intHouse = strHouse[0].toAscii() - 'A';
	dialHouse->setValue( intHouse );
}


DeviceSettingNexaBell::~DeviceSettingNexaBell()
{
}

void DeviceSettingNexaBell::saveParameters() {
	p_device->setParameter( "house", QString('A' + dialHouse->value()) );
}

void DeviceSettingNexaBell::houseChanged( int house )
{
	labelHouse->setText( QString('A' + house) );
}

