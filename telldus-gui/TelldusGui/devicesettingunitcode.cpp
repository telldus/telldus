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
#include "devicesettingunitcode.h"
#include "device.h"
#include <QGridLayout>
#include <QDial>
#include <QLabel>
#include <QDebug>

DeviceSettingUnitcode::DeviceSettingUnitcode(Device *device, QWidget *parent)
 : DeviceSetting(device, parent),
	dialCode(0)
{
	QGridLayout *gridLayout = new QGridLayout(this);

	QLabel *labelCodeTitle = new QLabel(this);
	labelCodeTitle->setObjectName(QString::fromUtf8("labelCodeTitle"));
	labelCodeTitle->setAlignment(Qt::AlignCenter);
	labelCodeTitle->setText( tr("Unit code") );
	gridLayout->addWidget(labelCodeTitle, 0, 0, 1, 1);

	dialCode = new QDial(this);
	dialCode->setObjectName(QString::fromUtf8("dialCode"));
	dialCode->setMinimum(1);
	dialCode->setMaximum(4);
	dialCode->setPageStep(1);
	dialCode->setOrientation(Qt::Horizontal);
	dialCode->setNotchesVisible(true);
	gridLayout->addWidget(dialCode, 1, 0, 1, 1);
	connect(dialCode, SIGNAL(valueChanged(int)), this, SLOT(codeChanged(int) ) );


	labelCode = new QLabel(this);
	labelCode->setObjectName(QString::fromUtf8("labelCode"));
	labelCode->setAlignment(Qt::AlignCenter);
	labelCode->setText( "1" );
	gridLayout->addWidget(labelCode, 2, 0, 1, 1);

	this->setValue( "unit", device->parameter("unit", "1") );
}


DeviceSettingUnitcode::~DeviceSettingUnitcode() {
}

void DeviceSettingUnitcode::saveParameters() {
	p_device->setParameter( "unit", QString::number(dialCode->value()) );
}

void DeviceSettingUnitcode::setValue( const QString &name, const QString &value ) {
	if (name == "unit") {
		dialCode->setValue(value.toInt());
	}
}

void DeviceSettingUnitcode::codeChanged(int code) {
	labelCode->setText(QString("%1").arg(dialCode->value()));
}
