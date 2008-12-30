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
#ifndef DEVICESETTINGSARTANO_H
#define DEVICESETTINGSARTANO_H

#include <devicesetting.h>
#include <QHash>
#include <QRadioButton>

/**
 * @author Micke Prag <micke.prag@telldus.se>
 */
class DeviceSettingSartano : public DeviceSetting
{
Q_OBJECT
public:
	DeviceSettingSartano(Device *device, QWidget *parent = 0);

	virtual ~DeviceSettingSartano();

public slots:
	virtual void saveParameters();

protected:
	QHash<int, QRadioButton *> button1;
	QHash<int, QRadioButton *> button0;
};

#endif
