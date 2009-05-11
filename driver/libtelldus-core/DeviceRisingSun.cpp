// #include "StdAfx.h"
#include "DeviceRisingSun.h"
#include <string.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>

using namespace TelldusCore;

/*
* Constructor
*/
DeviceRisingSun::DeviceRisingSun(int id, const std::string &model, const std::string &name)
	:Device(id, model, name),
	intHouse(0),
	intCode(0)
{
}

/*
* Destructor
*/
DeviceRisingSun::~DeviceRisingSun(void)
{}

bool DeviceRisingSun::setHouse(const std::string &newHouse) {
	if (newHouse.length() > 0) {
		intHouse = atoi(newHouse.c_str()) - 1;
	} else {
		intHouse = 0;
	}
	return true;
}

bool DeviceRisingSun::setUnit(const std::string &newUnit) {
	if (newUnit.length() > 0) {
		intCode = atoi(newUnit.c_str()) - 1;
	} else {
		intCode = 0;
	}
	return true;
}

bool DeviceRisingSun::setDeviceParameter(const std::string &strName, const std::string &strValue) {
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
int DeviceRisingSun::turnOn(void){

	try{
		std::string strCode = getStringCode(intHouse);
		std::string strUnit = getStringCode(intCode);
		strCode.append(strUnit);

		strCode.insert(0, "S.e");
		strCode.append("e..ee..ee..ee..e+");	//the "turn on"-code, keeps it like this, doesn't have to be regenerated each time

		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
* Turn off this device
*/
int DeviceRisingSun::turnOff(void){

	try{
		std::string strCode = getStringCode(intHouse);
		std::string strUnit = getStringCode(intCode);
		strCode.append(strUnit);

		strCode.insert(0, "S.e");
		strCode.append("e..ee..ee..e.e.e+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time

		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
*	Convert an integer to byte string where 0 is represented by $k and 1 by k$, reversed and padded with 0's as needed
*/
std::string DeviceRisingSun::getStringCode(int intToConvert){

	std::string strReturn = "";

	try{
		for(int i = 0; i < 4; ++i) {
			if (i == intToConvert) {
				strReturn.append( ".e.e" );
			} else {
				strReturn.append( "e..e" );
			}
		}
	}
	catch(...){
		throw;
	}
	return strReturn;

}

bool DeviceRisingSun::parameterMatches( const std::string &name, const std::string &value ) const {
	if (value.length() == 0) {
		return false;
	}
	
	return false;
}

/*
* Has the device got the method?
*/
int DeviceRisingSun::methods(int methodsSupported){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

std::string DeviceRisingSun::getProtocol() const {
	return "risingsun";
}
