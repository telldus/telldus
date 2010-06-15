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
#include "devicesettingupm.h"
#include "device.h"
#include <QGridLayout>
#include <QRadioButton>
#include <QLabel>
#include <QVariant>
#include <QDebug>

DeviceSettingUpm::DeviceSettingUpm(Device *device, QWidget *parent)
 : DeviceSetting(device, parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addSpacerItem( new QSpacerItem(20, 109, QSizePolicy::Minimum, QSizePolicy::Expanding) );
	QGridLayout *gl = new QGridLayout;

	QLabel *houseLabel = new QLabel( tr("House"), this);
	houseLabel->setAlignment( Qt::AlignCenter );
	gl->addWidget(houseLabel, 0, 0);

	house.setMinimum(0);
	house.setMaximum(4095);
	gl->addWidget(&house, 1, 0);

	QLabel *buttonLabel = new QLabel( tr("Button"), this);
	buttonLabel->setAlignment( Qt::AlignCenter );
	gl->addWidget(buttonLabel, 0, 1);

	int choosedButton = device->parameter("unit", "1").toInt();
	QVBoxLayout *vl = new QVBoxLayout;
	for (int i = 1; i <= 4; ++i) {
		QRadioButton *button = new QRadioButton( tr("Button %1").arg(i), this);
		if (i == 1 || choosedButton == i) {
			button->setChecked( true );
		}
		vl->addWidget(button);
	}
	gl->addLayout(vl, 1, 1);

	layout->addLayout( gl );
	layout->addSpacerItem( new QSpacerItem(20, 109, QSizePolicy::Minimum, QSizePolicy::Expanding) );

	house.setValue( device->parameter("house", "0").toInt() );
}

DeviceSettingUpm::~DeviceSettingUpm() {
}

void DeviceSettingUpm::saveParameters() {
	p_device->setParameter( "house", QString::number(house.value()) );
}

void DeviceSettingUpm::setValue( const QString &name, const QString &value ) {
	//TODO!
}
