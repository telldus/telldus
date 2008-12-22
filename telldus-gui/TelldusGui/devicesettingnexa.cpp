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
#include <QGridLayout>
#include <QDial>
#include <QLabel>

DeviceSettingNexa::DeviceSettingNexa(QWidget *parent)
 : DeviceSetting(parent)
{
	gridLayout1 = new QGridLayout(this);
	gridLayout1->setSpacing(6);
	gridLayout1->setMargin(9);
	gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
	
	dialCode = new QDial(this);
	dialCode->setObjectName(QString::fromUtf8("dialCode"));
	dialCode->setMinimum(1);
	dialCode->setMaximum(16);
	dialCode->setPageStep(1);
	dialCode->setOrientation(Qt::Horizontal);
	dialCode->setNotchesVisible(true);

	gridLayout1->addWidget(dialCode, 1, 1, 1, 1);

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

	label_2 = new QLabel(this);
	label_2->setObjectName(QString::fromUtf8("label_2"));
	label_2->setAlignment(Qt::AlignCenter);
	label_2->setText( tr("Unitcode") );

	gridLayout1->addWidget(label_2, 0, 1, 1, 1);

	labelHouse = new QLabel(this);
	labelHouse->setObjectName(QString::fromUtf8("labelHouse"));
	labelHouse->setAlignment(Qt::AlignCenter);
	labelHouse->setText( "A" );
	connect( dialHouse, SIGNAL( valueChanged( int ) ), this, SLOT( houseChanged(int) ) );

	gridLayout1->addWidget(labelHouse, 2, 0, 1, 1);

	label_4 = new QLabel(this);
	label_4->setObjectName(QString::fromUtf8("label_4"));
	label_4->setAlignment(Qt::AlignCenter);
	label_4->setText( "1" );
	connect( dialCode, SIGNAL(valueChanged(int)), label_4, SLOT(setNum(int)) );

	gridLayout1->addWidget(label_4, 2, 1, 1, 1);
}


DeviceSettingNexa::~DeviceSettingNexa()
{
}


/**
 * @fn DeviceSettingNexa::setDevice( DeviceInfo * )
 */
//void DeviceSettingNexa::setDevice( DeviceInfo *device )
//{
//	DeviceSetting::setDevice( device );
//	QString strHouse = device->setting("nexa_house", "A");
//	uint intHouse = strHouse[0].toAscii() - 'A';
//	dialHouse->setValue( intHouse );
// 	dialCode->setValue( device->setting("nexa_unit", "1").toInt() );
//}
//
///**
// * @fn DeviceSettingNexa::saveSetting()
// */
//void DeviceSettingNexa::saveSetting() {
//	p_dev->setSetting( "nexa_house", QString('A' + dialHouse->value()) );
//	p_dev->setSetting( "nexa_unit", QString::number(dialCode->value()) );
//}

/**
 * @fn DeviceSettingNexa::houseChanged( int house )
 */
void DeviceSettingNexa::houseChanged( int house )
{
	labelHouse->setText( QString('A' + house) );
}

