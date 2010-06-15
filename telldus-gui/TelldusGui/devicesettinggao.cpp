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
#include "devicesettinggao.h"
#include "device.h"
#include <QGridLayout>
#include <QDial>
#include <QLabel>
#include <QDebug>

DeviceSettingGAO::DeviceSettingGAO(Device *device, QWidget *parent)
 : DeviceSetting(device, parent),
	dialCode(0)
{
	QGridLayout *gridLayout = new QGridLayout(this);
	
	QLabel *labelCodeTitle = new QLabel(this);
	labelCodeTitle->setObjectName(QString::fromUtf8("labelCodeTitle"));
	labelCodeTitle->setAlignment(Qt::AlignCenter);
	labelCodeTitle->setText( tr("Unitcode") );
	gridLayout->addWidget(labelCodeTitle, 0, 0, 1, 1);
	
	dialCode = new QDial(this);
	dialCode->setObjectName(QString::fromUtf8("dialCode"));
	dialCode->setMinimum(0);
	dialCode->setMaximum(11);
	dialCode->setPageStep(1);
	dialCode->setOrientation(Qt::Horizontal);
	dialCode->setNotchesVisible(true);
	gridLayout->addWidget(dialCode, 1, 0, 1, 1);
	connect(dialCode, SIGNAL(valueChanged(int)), this, SLOT(codeChanged(int) ) );
	
	labelCode = new QLabel(this);
	labelCode->setObjectName(QString::fromUtf8("labelCode"));
	labelCode->setAlignment(Qt::AlignCenter);
	labelCode->setText( "A1" );
	gridLayout->addWidget(labelCode, 2, 0, 1, 1);
	
	this->setValue( "house", device->parameter("house", "A") );
	this->setValue( "unit", device->parameter("unit", "1") );
}


DeviceSettingGAO::~DeviceSettingGAO() {
}

void DeviceSettingGAO::saveParameters() {
	p_device->setParameter( "house", QString::number(this->house()) );
	p_device->setParameter( "unit", QString::number(this->unit()) );
}

void DeviceSettingGAO::setValue( const QString &name, const QString &value ) {
	if (name == "house") {
		uint intValue = value.toInt() - 1;
		intValue *= 3;
		intValue += this->unit() - 1;
		dialCode->setValue(intValue);
	} else if (name == "unit") {
		uint intValue = this->house() - 1;
		intValue *= 3;
		intValue += value.toInt() - 1;
		dialCode->setValue(intValue);
	}
}

void DeviceSettingGAO::codeChanged(int code) {
	int house = this->house();
	labelCode->setText(QString("%1%2").arg( QChar('A'+house-1) ).arg(this->unit()));
}

int DeviceSettingGAO::house() const {
	return dialCode->value() / 3 + 1;
}

int DeviceSettingGAO::unit() const {
	return dialCode->value() % 3 + 1;
}
