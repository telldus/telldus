//
// C++ Interface: devicesetting
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DEVICESETTING_H
#define DEVICESETTING_H

#include <QWidget>
//#include "deviceinfo.h"

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
class DeviceSetting : public QWidget
{
Q_OBJECT
public:
	DeviceSetting(QWidget *parent = 0);

	~DeviceSetting();

//public slots:
//	virtual void setDevice( DeviceInfo * );
//	virtual void saveSetting() = 0;

//protected:
//	DeviceInfo *p_dev;
};

#endif
