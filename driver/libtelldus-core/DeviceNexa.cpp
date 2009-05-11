// #include "StdAfx.h"
#include "DeviceNexa.h"
#include <string.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>

#ifdef _WINDOWS
#define strcasecmp _stricmp
#endif

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
		intHouse = newHouse[0] - 'A';
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
int DeviceNexa::turnOn(void){

	try{
		std::string strCode = getStringCode(intHouse);
		std::string strUnit = getStringCode(intCode);
		strCode.append(strUnit);

		strCode.insert(0, "S");
		strCode.append("$k$k$kk$$kk$$kk$$k+");	//the "turn on"-code, keeps it like this, doesn't have to be regenerated each time

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
int DeviceNexa::turnOff(void){

	try{
		std::string strCode = getStringCode(intHouse);
		std::string strUnit = getStringCode(intCode);
		strCode.append(strUnit);

		strCode.insert(0, "S");
		strCode.append("$k$k$kk$$kk$$k$k$k+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time

		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
* Send a bell
*/
int DeviceNexa::bell(void){

	try{
		std::string strCode = getStringCode(intHouse);

		strCode.append("$kk$$kk$$kk$$k$k"); //the unit-code is always 7, doesn't have to be regenerated each time
		strCode.insert(0, "S");
		strCode.append("$kk$$kk$$kk$$kk$$k+");	//the "bell"-code, keeps it like this, doesn't have to be regenerated each time

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
std::string DeviceNexa::getStringCode(int intToConvert){

	std::string strReturn = "";

	try{
		for( int i = 0; i < 4; ++i ) {
			if (intToConvert & 1) { //Convert 1
				strReturn.append("$kk$");
			} else { //Convert 0
				strReturn.append("$k$k");
			}
			intToConvert >>= 1;
			
		}
	}
	catch(...){
		throw;
	}
	return strReturn;

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
int DeviceNexa::methods(int methodsSupported){
	std::string strModel = this->getModel();
	
	if (strcasecmp(strModel.c_str(), "codeswitch") == 0 ||
		  strcasecmp(strModel.c_str(), "selflearning") == 0)
	{
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
	} else if (strcasecmp(strModel.c_str(), "bell") == 0) {
		if (methodsSupported & TELLSTICK_BELL) {
			return TELLSTICK_BELL;
		} else if (methodsSupported & TELLSTICK_TURNON) {
			return TELLSTICK_TURNON;
		}
	}
	return 0;
}

std::string DeviceNexa::getProtocol() const {
	return "arctech";
}
