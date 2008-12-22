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

#include <devicesetting.h>

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
	DeviceSettingNexa(QWidget *parent = 0);

	~DeviceSettingNexa();

//public slots:
//	virtual void setDevice( DeviceInfo * );
//	virtual void saveSetting();

protected slots:
	void houseChanged( int house );

protected:
	QGridLayout *gridLayout1;
	QDial *dialCode;
	QDial *dialHouse;	
	QLabel *label;
	QLabel *label_2;
	QLabel *labelHouse;
	QLabel *label_4;
};

#endif
