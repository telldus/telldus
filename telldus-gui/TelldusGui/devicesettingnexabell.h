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
#ifndef DEVICESETTINGNEXABELL_H
#define DEVICESETTINGNEXABELL_H

#include "devicesetting.h"

class QGridLayout;
class QDial;
class QLabel;

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class DeviceSettingNexaBell : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingNexaBell(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingNexaBell();

public slots:
	virtual void saveParameters();

protected slots:
	void houseChanged( int house );

protected:
	QDial *dialHouse;
	QLabel *labelHouse;
};

#endif
