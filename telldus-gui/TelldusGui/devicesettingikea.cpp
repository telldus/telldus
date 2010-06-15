//
// C++ Implementation: devicesettingikea
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "devicesettingikea.h"
#include "device.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>

DeviceSettingIkea::DeviceSettingIkea(Device *device, QWidget *parent)
 : DeviceSetting(device, parent)
{
	QVBoxLayout *layout = new QVBoxLayout( this );
	QGridLayout *gl = new QGridLayout;

	// System
	QLabel *l = new QLabel(tr("System"), this);
	l->setAlignment( Qt::AlignHCenter );
	gl->addWidget( l, 0, 0 );

	spinBox = new QSpinBox( this );
	spinBox->setMinimum( 1 );
	spinBox->setMaximum( 16 );
	gl->addWidget( spinBox, 1, 0 );

	// Units
	l = new QLabel(tr("Units"), this);
	l->setAlignment( Qt::AlignHCenter );
	gl->addWidget( l, 0, 1 );

	QGridLayout *bl = new QGridLayout;
	for ( int i = 0; i < 10; ++i ) {
		unit[i] = new QCheckBox(this);
		if (i > 0) {
			bl->addWidget( unit[i], 0, i-1 );
			bl->addWidget( new QLabel(QString::number(i), this), 1, i-1 );
		} else {
			bl->addWidget( unit[i], 0, 9 );
			bl->addWidget( new QLabel("10", this), 1, 9 );
		}
	}
	gl->addLayout( bl, 1, 1 );

	//Fade
	l = new QLabel(tr("Fade"), this);
	l->setAlignment( Qt::AlignHCenter );
	gl->addWidget( l, 0, 2 );

	QVBoxLayout *vl = new QVBoxLayout;
	smooth = new QRadioButton( tr("Smooth"), this );
	vl->addWidget( smooth );
	QRadioButton *instant = new QRadioButton( tr("Instant"), this );
	instant->setChecked( true );
	vl->addWidget( instant );
	gl->addLayout( vl, 1, 2 );

	layout->addLayout( gl );
	layout->addStretch( 1 );

	//Restore the settings
	this->setValue( "system", device->parameter("system", "1") );
	this->setValue( "units", device->parameter("units", "") );
	this->setValue( "fade", device->parameter("fade", "false") );
}


DeviceSettingIkea::~DeviceSettingIkea()
{
}

void DeviceSettingIkea::saveParameters() {
	p_device->setParameter( "system", QString::number(spinBox->value()) ); //System

	QStringList units;
	for (int i = 0; i < 10; ++i) {
		if (unit[i]->isChecked()) {
			if (i == 0) {
				units << "10";
			} else {
				units << QString::number(i);
			}
		}
	}
	p_device->setParameter( "units", units.join(",") );

	p_device->setParameter( "fade", (smooth->isChecked() ? "true" : "false") );
}

void DeviceSettingIkea::setValue( const QString &name, const QString &value ) {
	if (name == "system") {
		spinBox->setValue( value.toInt() );
	} else if (name == "units") {
		if (value != "") {
			QStringList units = value.split(",");
			for(int i = 0; i < units.size(); ++i) {
				int intUnit = units.at(i).toInt();
				if (intUnit == 10) {
					intUnit = 0;
				} else if (intUnit > 10) {
					continue;
				}
				unit[intUnit]->setChecked(true);
			}
		}
	} else if (name == "fade") {
		if (value.compare("true", Qt::CaseInsensitive) == 0) {
			smooth->setChecked( true );
		} else {
			smooth->setChecked( false );
		}
	}
}
