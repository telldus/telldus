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
#ifndef DEVICESETTINGGAO_H
#define DEVICESETTINGGAO_H

#include "devicesetting.h"

class QDial;
class QLabel;

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class DeviceSettingGAO : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingGAO(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingGAO();

public slots:
	virtual void saveParameters();
	virtual void setValue( const QString &name, const QString &value );
	
private slots:
	void codeChanged(int);

private:
	int house() const;
	int unit() const;

protected:
	QDial *dialCode;
	QLabel *labelCode;
};

#endif
