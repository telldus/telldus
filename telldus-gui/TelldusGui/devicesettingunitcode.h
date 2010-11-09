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
#ifndef DEVICESETTINGUNITCODE_H
#define DEVICESETTINGUNITCODE_H

#include "devicesetting.h"

class QDial;
class QLabel;

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class DeviceSettingUnitcode : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingUnitcode(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingUnitcode();

public slots:
	virtual void saveParameters();
	virtual void setValue( const QString &name, const QString &value );

private slots:
	void codeChanged(int);

protected:
	QDial *dialCode;
	QLabel *labelCode;
};

#endif
