#include "DeviceFuhaote.h"
//#include <string>
//#include <bitset>
//#include <algorithm>
//#include <fstream>

using namespace TelldusCore;

const char S = 19;
const char L = 58;
const char B0[] = {S,L,L,S,0};
const char B1[] = {L,S,L,S,0};
const char OFF[] = {S,L,S,L,S,L,L,S,0};
const char ON[]  = {S,L,L,S,S,L,S,L,0};

/*
* Constructor
*/
DeviceFuhaote::DeviceFuhaote(int id, const std::string &model, const std::string &name)
	:Device(id, model, name)
{
}


DeviceFuhaote::~DeviceFuhaote(void)
{
}

bool DeviceFuhaote::setCode(const std::string &strNewCode) {
	strCode = strNewCode;
	return true;
}

bool DeviceFuhaote::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("code") == 0) {
		return setCode(strValue);
	}
	return false;
}

/*
* Turn on this device
*/
int DeviceFuhaote::turnOn(void){

	std::string strCode = "S";
	strCode.append(getStringCode());
	strCode.append(ON);
	strCode.append(1, S);
	strCode.append("+");
	
	return Device::send(strCode);
}

/*
* Turn off this device
*/
int DeviceFuhaote::turnOff(void){
	
	std::string strCode = "S";
	strCode.append(getStringCode());
	strCode.append(OFF);
	strCode.append(1, S);
	strCode.append("+");
	
	return Device::send(strCode);
}

bool DeviceFuhaote::parameterMatches( const std::string &name, const std::string &value ) const {
	if (name.compare("code") == 0 && value.compare(strCode) == 0) {
		return true;
	}
	return false;
}

/*
* Has the device got the method?
*/
int DeviceFuhaote::methods(){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

std::string DeviceFuhaote::getStringCode(void){
	
	std::string strReturn;

	//House code
	for(size_t i = 0; i < 5; ++i) {
		if (strCode[i] == '0') {
			strReturn.append(B0);
		} else if (strCode[i] == '1') {
			strReturn.append(B1);
		}
	}
	//Unit code
	for(size_t i = 5; i < 10; ++i) {
		if (strCode[i] == '0') {
			strReturn.append(B0);
		} else if (strCode[i] == '1') {
			strReturn.append(1, S);
			strReturn.append(1, L);
			strReturn.append(1, S);
			strReturn.append(1, L);
		}
	}
	return strReturn;

}

std::string DeviceFuhaote::getProtocol() const {
	return "fuhaote";
}

