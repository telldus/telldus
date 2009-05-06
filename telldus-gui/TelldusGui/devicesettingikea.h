//
// C++ Interface: devicesettingikea
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DEVICESETTINGIKEA_H
#define DEVICESETTINGIKEA_H

#include "devicesetting.h"
#include <QHash>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>

/**
 *	@author Micke Prag <micke.prag@telldus.se>
 **/
class DeviceSettingIkea : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingIkea(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingIkea();

public slots:
	virtual void saveParameters();

protected:
	QSpinBox	*spinBox;
	QHash<int, QCheckBox *> unit;
	QRadioButton *smooth;
};

#endif
