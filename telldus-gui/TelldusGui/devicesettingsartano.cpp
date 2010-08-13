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
#include "devicesettingsartano.h"
#include "device.h"
#include <QButtonGroup>
#include <QRadioButton>
#include <QGridLayout>
#include <QLabel>

DeviceSettingSartano::DeviceSettingSartano(Device *device, QWidget *parent)
 : DeviceSetting(device, parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	QGridLayout *gl = new QGridLayout;

	for( int i = 0; i < 10; ++i ) {
		QButtonGroup *g = new QButtonGroup(this);
		button1[i] = new QRadioButton(this);
		button0[i] = new QRadioButton(this);
		button0[i]->setChecked( true );
		g->addButton(button1[i]);
		g->addButton(button0[i]);

		gl->addWidget(button1[i], 1, i);
		gl->addWidget(button0[i], 0, i);
		QLabel *label = 0;
		if (i < 5) {
			label = new QLabel(QString::number(i+1), this);
		} else {
			label = new QLabel(QString('A'+i-5), this);
		}
		gl->addWidget( label, 2, i );
	}

	layout->addLayout( gl );
	layout->addStretch( 1 );

	this->setValue("code", device->parameter("code", "0000000000"));
}

DeviceSettingSartano::~DeviceSettingSartano() {
}

void DeviceSettingSartano::saveParameters() {
	QString setting;
	for (int i = 0; i < 10; ++i) {
		if (button0[i]->isChecked()) {
			setting.append("1");
		} else {
			setting.append("0");
		}
	}
	p_device->setParameter( "code", setting );
}

void DeviceSettingSartano::setValue( const QString &name, const QString &value ) {
	if (name == "code") {
		for (int i = 0; i < 10; ++i) {
			if (value[i] == '0') {
				button1[i]->setChecked(true);
			} else {
				button0[i]->setChecked(true);
			}
		}
	}
}
