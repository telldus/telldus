#include "DeviceBrateck.h"
#include <string>
#include <bitset>
#include <algorithm>
#include <fstream>

using namespace TelldusCore;

const char S = '!';
const char L = 'V';
const char B1[] = {L,S,L,S,0};
const char BX[] = {S,L,L,S,0};
const char B0[] = {S,L,S,L,0};
const char BUP[] = {L,S,L,S,S,L,S,L,S,L,S,L,S,L,S,L,S,0};
const char BSTOP[] = {S,L,S,L,L,S,L,S,S,L,S,L,S,L,S,L,S,0};
const char BDOWN[] = {S,L,S,L,S,L,S,L,S,L,S,L,L,S,L,S,S,0};

/*
* Constructor
*/
DeviceBrateck::DeviceBrateck(int id, const std::string &model, const std::string &name)
	:Device(id, model, name)
{
}


DeviceBrateck::~DeviceBrateck(void)
{
}

bool DeviceBrateck::setHouse(const std::string &strNewHouse) {
	strHouse = strNewHouse;
	return true;
}

bool DeviceBrateck::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("house") == 0) {
		return setHouse(strValue);
	}
	return false;
}

/*
* Turn on this device
*/
int DeviceBrateck::turnOn(void){

	try{
		std::string strCode = getStringCode();
		
		strCode.insert(0, "S");
		strCode.append(BUP);
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
int DeviceBrateck::turnOff(void){
	
	try{
		std::string strCode = getStringCode();
		
		strCode.insert(0, "S");
		strCode.append(BDOWN);
		strCode.append("+");	//the "turn off"-code!
		
		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

bool DeviceBrateck::parameterMatches( const std::string &name, const std::string &value ) const {
	return false;
}

/*
* Has the device got the method?
*/
int DeviceBrateck::methods(){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

std::string DeviceBrateck::getStringCode(void){
	
	std::string strReturn;

	try{
		for( size_t i = 0; i < strHouse.length(); ++i ) {
			if (strHouse[i] == '1') {
				strReturn.insert(0, B1);
			} else if (strHouse[i] == '-') {
				strReturn.insert(0, BX);
			} else if (strHouse[i] == '0') {
				strReturn.insert(0, B0);
			}
		}
	}
	catch(...){
		throw;
	}
	return strReturn;

}

std::string DeviceBrateck::getProtocol() const {
	return "brateck";
}

