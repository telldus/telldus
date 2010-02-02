//
// C++ Implementation: devicesetting
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "devicesetting.h"

DeviceSetting::DeviceSetting(Device *device, QWidget *parent)
	: QWidget(parent),
	p_device(device)
{
}


DeviceSetting::~DeviceSetting()
{
}
