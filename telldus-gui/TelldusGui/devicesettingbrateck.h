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
#ifndef DEVICESETTINGBRATECK_H
#define DEVICESETTINGBRATECK_H

#include "devicesetting.h"
#include <QHash>

class QSlider;
class QLabel;

/**
 * @author Micke Prag <micke.prag@telldus.se>
 */
class DeviceSettingBrateck : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingBrateck(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingBrateck();

public slots:
	virtual void saveParameters();

private slots:
	void sliderChanged( int );

protected:
	QHash<int, QSlider *> slider;
	QHash<int, QLabel *> label;
	QString setting;
};

#endif //DEVICESETTINGBRATECK_H
