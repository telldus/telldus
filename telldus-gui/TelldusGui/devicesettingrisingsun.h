//
// C++ Interface: devicesettingnexa
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DEVICESETTINGRISINGSUN_H
#define DEVICESETTINGRISINGSUN_H

#include "devicesetting.h"

class QGridLayout;
class QDial;
class QLabel;

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class DeviceSettingRisingSun : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingRisingSun(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingRisingSun();

public slots:
	virtual void saveParameters();

protected:
	QDial *dialHouse;
	QDial *dialUnit;
};

#endif
