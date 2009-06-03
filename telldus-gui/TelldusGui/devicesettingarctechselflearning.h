//
// C++ Interface: devicesettingnexa
//
// Description:
//
//
// Author: Fredrik Jacobsson <fredrik.jacobsson@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DEVICESETTINGARCTECHSELFLEARNING_H
#define DEVICESETTINGARCTECHSELFLEARNING_H

#include "devicesetting.h"

class QGridLayout;
class QSpinBox;
class QLabel;

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class DeviceSettingArctechSelflearning : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingArctechSelflearning(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingArctechSelflearning();

public slots:
	virtual void saveParameters();

protected:
	QSpinBox *spinRemotecode;
	QSpinBox *spinUnitcode;
};

#endif
