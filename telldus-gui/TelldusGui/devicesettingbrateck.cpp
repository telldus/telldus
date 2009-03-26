//
// C++ Implementation: devicesettingsartano
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "devicesettingbrateck.h"
#include "device.h"
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QVariant>
#include <QDebug>

DeviceSettingBrateck::DeviceSettingBrateck(Device *device, QWidget *parent)
 : DeviceSetting(device, parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addSpacerItem( new QSpacerItem(20, 109, QSizePolicy::Minimum, QSizePolicy::Expanding) );
	QGridLayout *gl = new QGridLayout;

	for( int i = 0; i < 8; ++i ) {
		slider[i] = new QSlider(this);
		slider[i]->setMinimum(0);
		slider[i]->setMaximum(2);
		slider[i]->setPageStep(1);
		slider[i]->setValue(1);
		slider[i]->setProperty( "slider_index", i );

		label[i] = new QLabel("-", this);
		label[i]->setAlignment( Qt::AlignCenter );

		connect( slider[i], SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)) );

		gl->addWidget(slider[i], 0, i);
		gl->addWidget(label[i], 1, i);
	}

	layout->addLayout( gl );
	layout->addSpacerItem( new QSpacerItem(20, 109, QSizePolicy::Minimum, QSizePolicy::Expanding) );

	setting = device->parameter("nexa_house", "00000000");
	setting.truncate(8); //Make sure it is the correct length
	for (int i = 0; i < 8; ++i) {
		if (setting[i] == '0') {
			slider[i]->setValue(0);
		} else if (setting[i] == '-') {
			slider[i]->setValue(1);
		} else if (setting[i] == '1') {
			slider[i]->setValue(2);
		}
	}
}

DeviceSettingBrateck::~DeviceSettingBrateck() {
}

void DeviceSettingBrateck::saveParameters() {
	p_device->setParameter( "nexa_house", setting );
}

void DeviceSettingBrateck::sliderChanged( int value ) {
	QSlider *slider = qobject_cast<QSlider *>(sender());
	if (!slider) {
		return;
	}
	int index = slider->property("slider_index").toInt();
	if (!label.contains(index)) {
		return;
	}
	QChar v;
	if (value == 0) {
		v = '0';
	} else if (value == 1) {
		v = '-';
	} else if (value == 2) {
		v = '1';
	}
	label[index]->setText( v );
	setting[index] = v;
}
