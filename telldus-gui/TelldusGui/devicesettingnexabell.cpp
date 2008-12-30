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
	gridLayout1 = new QGridLayout(this);
	gridLayout1->setSpacing(6);
	gridLayout1->setMargin(9);
	gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));

	dialHouse = new QDial(this);
	dialHouse->setObjectName(QString::fromUtf8("dialHouse"));
	dialHouse->setMinimum(0);
	dialHouse->setMaximum(15);
	dialHouse->setPageStep(1);
	dialHouse->setOrientation(Qt::Horizontal);
	dialHouse->setNotchesVisible(true);

	gridLayout1->addWidget(dialHouse, 1, 0, 1, 1);

	label = new QLabel(this);
	label->setObjectName(QString::fromUtf8("label"));
	label->setAlignment(Qt::AlignCenter);
	label->setText( tr("Housecode") );

	gridLayout1->addWidget(label, 0, 0, 1, 1);

	labelHouse = new QLabel(this);
	labelHouse->setObjectName(QString::fromUtf8("labelHouse"));
	labelHouse->setAlignment(Qt::AlignCenter);
	labelHouse->setText( "A" );
	connect( dialHouse, SIGNAL( valueChanged( int ) ), this, SLOT( houseChanged(int) ) );

	gridLayout1->addWidget(labelHouse, 2, 0, 1, 1);

}


DeviceSettingNexaBell::~DeviceSettingNexaBell()
{
}


/**
void DeviceSettingNexaBell::setDevice( DeviceInfo *device )
{
	DeviceSetting::setDevice( device );
	QString strHouse = device->setting("nexa_house", "A");
	uint intHouse = strHouse[0].toAscii() - 'A';
	dialHouse->setValue( intHouse );
}
*/

void DeviceSettingNexaBell::saveParameters() {
	p_device->setParameter( "nexa_house", QString('A' + dialHouse->value()) );
}

/**
 * @fn DeviceSettingNexaBell::houseChanged( int house )
 */
void DeviceSettingNexaBell::houseChanged( int house )
{
	labelHouse->setText( QString('A' + house) );
}

