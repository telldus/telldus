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
#ifndef DEVICESETTINGSELFLEARNING_H
#define DEVICESETTINGSELFLEARNING_H

#include "devicesetting.h"

class QGridLayout;
class QSpinBox;
class QLabel;

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class DeviceSettingSelflearning : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingSelflearning(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingSelflearning();

	virtual void setRemoteMinMax(int min, int max);

public slots:
	virtual void saveParameters();
	virtual void setValue( const QString &name, const QString &value );

protected slots:
	void randomizeCode();

protected:
	QSpinBox *spinRemotecode;
};

#endif
