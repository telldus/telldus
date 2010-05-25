// #include "StdAfx.h"
#include "DeviceRisingSun.h"
#include <string.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>
#include "common.h"

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
		if (isSelflearning()) {
			return Device::send(getStringSelflearning(true));

		} else {
			std::string strCode = getStringCodeswitch(intHouse);
			std::string strUnit = getStringCodeswitch(intCode);
			strCode.append(strUnit);
	
			strCode.insert(0, "S.e");
			strCode.append("e..ee..ee..ee..e+");	//the "turn on"-code, keeps it like this, doesn't have to be regenerated each time
	
			return Device::send(strCode);
		}
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
		if (isSelflearning()) {
			return Device::send(getStringSelflearning(false));

		} else {
			std::string strCode = getStringCodeswitch(intHouse);
			std::string strUnit = getStringCodeswitch(intCode);
			strCode.append(strUnit);
	
			strCode.insert(0, "S.e");
			strCode.append("e..ee..ee..e.e.e+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time
	
			return Device::send(strCode);
		}
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

int DeviceRisingSun::learn(void){
	std::string strCode = "R";
	strCode.append( 1, 50 );
	strCode.append(getStringSelflearning(true));
	return Device::send(strCode);
}

/*
*	Convert an integer to byte string where 0 is represented by $k and 1 by k$, reversed and padded with 0's as needed
*/
std::string DeviceRisingSun::getStringCodeswitch(int intToConvert){

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

std::string DeviceRisingSun::getStringSelflearning(bool on){
	const char code_on[][7] = {
		"110110", "001110", "100110", "010110",
		"111001", "000101", "101001", "011001",
		"110000", "001000", "100000", "010000",
		"111100", "000010", "101100", "011100"
	};
	const char code_off[][7] = {
		"111110", "000001", "101110", "011110",
		"110101", "001101", "100101", "010101",
		"111000", "000100", "101000", "011000",
		"110010", "001010", "100010", "010010"
	};
	const char l = 120;
	const char s = 51;

	std::string strCode = "10";
	int code = intCode;
	code = (code < 0 ? 0 : code);
	code = (code > 15 ? 15 : code);
	if (on) {
		strCode.append(code_on[code]);
	} else {
		strCode.append(code_off[code]);
	}

	int house = intHouse;
	for(int i = 0; i < 25; ++i) {
		if (house & 1) {
			strCode.append(1, '1');
		} else {
			strCode.append(1, '0');
		}
		house >>= 1;
	}

	std::string strReturn;
	for(unsigned int i = 0; i < strCode.length(); ++i) {
		if (strCode[i] == '1') {
			strReturn.append(1, l);
			strReturn.append(1, s);
		} else {
			strReturn.append(1, s);
			strReturn.append(1, l);
		}
	}

	std::string prefix = "P";
	prefix.append(1, 5);
	prefix.append("S");
	strReturn.insert(0, prefix);
	strReturn.append(1, '+');
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
int DeviceRisingSun::methods() {
	std::string strModel = this->getModel();

	if (strcasecmp(strModel.c_str(), "selflearning") == 0) {
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_LEARN);
	}
	
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

std::string DeviceRisingSun::getProtocol() const {
	return "risingsun";
}

bool DeviceRisingSun::isSelflearning() const {
	return (strcasecmp(getModel().c_str(), "selflearning") == 0);
}

