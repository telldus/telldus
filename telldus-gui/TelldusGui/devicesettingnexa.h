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
#ifndef DEVICESETTINGNEXA_H
#define DEVICESETTINGNEXA_H

#include "devicesetting.h"

class QGridLayout;
class QDial;
class QLabel;

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class DeviceSettingNexa : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingNexa(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingNexa();

public slots:
	virtual void saveParameters();
	virtual void setValue( const QString &name, const QString &value );

protected slots:
	void houseChanged( int house );

protected:
	QDial *dialHouse;
	QDial *dialUnit;
	QLabel *labelHouse;
	QLabel *labelUnit;
};

#endif
