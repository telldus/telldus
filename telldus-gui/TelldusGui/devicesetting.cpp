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

void DeviceSetting::addProtocolMatch( const QString &protocol, const QString &model ){
	QString protocolmodel; //concat to one string
	protocolmodel.append(protocol).append(model);
	acceptedProtocolModel << protocolmodel;
}

void DeviceSetting::setProtocolValue( const QString &name, const QString &value, const QString &protocol, const QString &model ){
	QString protocolmodel; //concat to one string
	protocolmodel.append(protocol).append(model);
	if(acceptedProtocolModel.contains(protocolmodel)){  //protocol not set yet, or equal
		this->setValue(name, value);
	}
}
