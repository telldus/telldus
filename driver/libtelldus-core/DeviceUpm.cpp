#include "DeviceUpm.h"
#include <string>
#include <bitset>
#include <algorithm>
#include <fstream>

using namespace TelldusCore;

const char S = ';';
const char L = '~';
const char START[] = {S,0};
const char B1[] = {L,S,0};
const char B0[] = {S,L,0};
const char BON[] = {S,L,L,S,0};
const char BOFF[] = {S,L,S,L,0};

/*
* Constructor
*/
DeviceUpm::DeviceUpm(int id, const std::string &model, const std::string &name)
	:Device(id, model, name)
{
}


DeviceUpm::~DeviceUpm(void)
{
}

bool DeviceUpm::setHouse(const std::string &strHouse) {
	if (strHouse.length() > 0) {
		intHouse = atoi(strHouse.c_str());
	} else {
		intHouse = 0;
	}
	return true;
}

bool DeviceUpm::setUnit(const std::string &strUnit) {
	if (strUnit.length() > 0) {
		intUnit = atoi(strUnit.c_str()) - 1;
	} else {
		intUnit = 0;
	}
	if (intUnit < 0) {
		intUnit = 0;
	} else if (intUnit > 3) {
		intUnit = 3;
	}
	return true;
}

bool DeviceUpm::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("house") == 0) {
		return setHouse(strValue);
	}
	if (strName.compare("unit") == 0) {
		return setUnit(strValue);
	}
	return false;
}

/*
* Turn on this device
*/
int DeviceUpm::turnOn(void){

	try{
		std::string strCode = getStringCode(true);
		
		strCode.insert(0, "S");
		strCode.append("+");	//the "turn on"-code!
		
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
int DeviceUpm::turnOff(void){
	
	try{
		std::string strCode = getStringCode(false);
		
		strCode.insert(0, "S");
		strCode.append("+");	//the "turn off"-code!
		
		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

bool DeviceUpm::parameterMatches( const std::string &name, const std::string &value ) const {
	return false;
}

/*
* Has the device got the method?
*/
int DeviceUpm::methods(){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

std::string DeviceUpm::getStringCode(bool on){
	
	std::string strReturn;

	try{
		int code = intHouse;
		for( size_t i = 0; i < 12; ++i ) {
			if (code & 1) {
				strReturn.insert(0, B1);
			} else {
				strReturn.insert(0, B0);
			}
			code >>= 1;
		}
		strReturn.insert(0, START); //Startcode, first
		
		code = 0;
		if (on) {
			code += 2;
		}
		code <<= 2;
		code += intUnit;
		
		int check1 = 0, check2 = 0;
		for( size_t i = 0; i < 6; ++i ) {
			if (code & 1) {
				if (i % 2 == 0) {
					check1++;
				} else {
					check2++;
				}
			}
			if (code & 1) {
				strReturn.append(B1);
			} else {
				strReturn.append(B0);
			}
			code >>= 1;
		}
		
		if (check1 % 2 == 0) {
			strReturn.append(B0);
		} else {
			strReturn.append(B1);
		}
		if (check2 % 2 == 0) {
			strReturn.append(B0);
		} else {
			strReturn.append(B1);
		}
	}
	catch(...){
		throw;
	}
	return strReturn;

}

std::string DeviceUpm::getProtocol() const {
	return "upm";
}

