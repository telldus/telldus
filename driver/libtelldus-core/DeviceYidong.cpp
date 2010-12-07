#include "DeviceYidong.h"
#include <stdlib.h>

using namespace TelldusCore;

DeviceYidong::DeviceYidong(int id, const std::string &model, const std::string &name)
	:Device(id, model, name)
{
}


DeviceYidong::~DeviceYidong(void)
{
}

std::string DeviceYidong::getProtocol() const {
	return "yidong";
}

bool DeviceYidong::parameterMatches( const std::string &name, const std::string &value ) const {
	return false;
}

int DeviceYidong::methods(){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

bool DeviceYidong::setUnit(const std::string &newUnit) {
	if (newUnit.length() > 0) {
		intCode = atoi(newUnit.c_str());
	} else {
		intCode = 1;
	}
	return true;
}

bool DeviceYidong::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("unit") == 0) {
		return setUnit(strValue);
	}
	return false;
}

int DeviceYidong::turnOn(void){

	try{
		std::string strCode = getStringCode();
		
		strCode.insert(0, "S");
		strCode.append("$k$k$kk$$k+");
		
		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

int DeviceYidong::turnOff(void){
	
	try{
		std::string strCode = getStringCode();
		
		strCode.insert(0, "S");
		strCode.append("$kk$$k$k$k+");
		
		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

std::string DeviceYidong::getStringCode(void) {
	std::string strCode = "$k$k$k$k$k$k";

	switch(intCode) {
	case 1:
		strCode.append("$kk$$kk$$k$k$kk$");
		break;
	case 2:
		strCode.append("$kk$$kk$$kk$$k$k");
		break;
	case 3:
		strCode.append("$kk$$k$k$kk$$kk$");
		break;
	case 4:
		strCode.append("$k$k$kk$$kk$$kk$");
		break;
	}

	strCode.append("$k$k$k$k$kk$");
	return strCode;
}
