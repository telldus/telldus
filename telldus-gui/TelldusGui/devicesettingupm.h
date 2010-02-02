//
// C++ Interface: devicesettingsartano
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DEVICESETTINGUPM_H
#define DEVICESETTINGUPM_H

#include "devicesetting.h"
#include <QSpinBox>

/**
 * @author Micke Prag <micke.prag@telldus.se>
 */
class DeviceSettingUpm : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingUpm(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingUpm();

public slots:
	virtual void saveParameters();

protected:
	QSpinBox house;
};

#endif //DEVICESETTINGUPM_H
