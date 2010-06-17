// #include "StdAfx.h"
#include "DeviceNexa.h"
#include <string.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "common.h"

using namespace TelldusCore;

/*
* Constructor
*/
DeviceNexa::DeviceNexa(int id, const std::string &model, const std::string &name)
	:Device(id, model, name),
	intHouse(0),
	intCode(0)
{
}

/*
* Destructor
*/
DeviceNexa::~DeviceNexa(void)
{}

bool DeviceNexa::setHouse(const std::string &newHouse) {
	if (newHouse.length() > 0) {
		intHouse = atoi(newHouse.c_str());
	} else {
		intHouse = 0;
	}
	return true;
}

bool DeviceNexa::setUnit(const std::string &newUnit) {
	if (newUnit.length() > 0) {
		intCode = atoi(newUnit.c_str()) - 1;
	} else {
		intCode = 0;
	}
	return true;
}

bool DeviceNexa::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("house") == 0) {
		return setHouse(strValue);
	} else if (strName.compare("unit") == 0) {
		return setUnit(strValue);
	}
	return false;
}

/*
* Turn on this device
*/
int DeviceNexa::turnOn(void) {
	std::string strCode = "";
	if (isDimmer()) {
		return dim(255);
	} else {
		strCode = getStringSelflearning(false, 0);
		strCode.append( 1, 1 ); //On
		strCode.append( ":" );
	}
	return Device::send(strCode);
}

/*
* Turn off this device
*/
int DeviceNexa::turnOff(void){
	std::string strCode = getStringSelflearning(false, 0);
	strCode.append( 1, 2 ); //Off
	strCode.append( ":" );
	return Device::send(strCode);
}

/*
* Dim this device
*/
int DeviceNexa::dim(unsigned char level) {
	std::string strCode = getStringSelflearning(true, level);
	return Device::send(strCode);
}


int DeviceNexa::learn(void) {
	std::string strCode = getStringSelflearning(false, 0);
	strCode.append( 1, 1 ); //On
	strCode.append( ":" );
	return Device::send(strCode);
}


std::string DeviceNexa::getStringSelflearning(bool dim, unsigned char level) {
	std::string strMessage;
	strMessage.append( ":" );
	strMessage.append( 1, 15);
	strMessage.append( ":SEND:ATSL:" );
	int c = intHouse << 4;
	c = c | (intCode & 0xFF);
	strMessage.append( 1, (c >> 24) & 0xFF );
	strMessage.append( 1, (c >> 16) & 0xFF );
	strMessage.append( 1, (c >> 8) & 0xFF );
	strMessage.append( 1, c & 0xFF );
	return strMessage;
}

bool DeviceNexa::parameterMatches( const std::string &name, const std::string &value ) const {
	if (value.length() == 0) {
		return false;
	}
	
	if (name.compare("arctech_house") == 0) {
		return intHouse == value[0] - 'A';
	} else if (name.compare("arctech_unit") == 0) {
		return intCode == atoi(value.c_str()) - 1;
	}
	return true;
}

/*
* Has the device got the method?
*/
int DeviceNexa::methods(){
	std::string strModel = this->getModel();
	
	if (strcasecmp(strModel.c_str(), "selflearning-switch") == 0) {
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_LEARN);
		
	} else if (strcasecmp(strModel.c_str(), "selflearning-dimmer") == 0) {
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM | TELLSTICK_LEARN);
		
	}
	return 0;
}

bool DeviceNexa::isSelflearning() const {
	return true;
}

bool DeviceNexa::isDimmer() const {
	return strcasecmp(getModel().c_str(), "selflearning-dimmer") == 0;
}

std::string DeviceNexa::getProtocol() const {
	return "arctech";
}
