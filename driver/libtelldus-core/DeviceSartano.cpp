#include "DeviceSartano.h"
#include "DeviceNexa.h"
#include <string>
#include <bitset>
#include <algorithm>
#include <fstream>

using namespace TelldusCore;

/*
* Constructor
*/
DeviceSartano::DeviceSartano(int id, const std::string &model, const std::string &name)
	:Device(id, model, name)
{
}


DeviceSartano::~DeviceSartano(void)
{
}

bool DeviceSartano::setCode(const std::string &strNewCode) {
	strCode = strNewCode;
	return true;
}

bool DeviceSartano::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("code") == 0) {
		return setCode(strValue);
	}
	return false;
}

/*
* Turn on this device
*/
int DeviceSartano::turnOn(void){

	try{
		std::string strCode = getStringCode();
		
		strCode.insert(0, "S");
		strCode.append("$k$k$kk$$k+");	//the "turn on"-code, keeps it like this, doesn't have to be regenerated each time
		
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
int DeviceSartano::turnOff(void){
	
	try{
		std::string strCode = getStringCode();
		
		strCode.insert(0, "S");
		strCode.append("$kk$$k$k$k+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time
		
		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

bool DeviceSartano::parameterMatches( const std::string &name, const std::string &value ) const {
	return false;
}

/*
* Has the device got the method?
*/
int DeviceSartano::methods(){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

/*
*	Convert an integer to byte string where 0 is represented by $k and 1 by k$, reversed and padded with 0's as needed
*/
std::string DeviceSartano::getStringCode(void){
	
	std::string strReturn = strCode;

	try{
		size_t intPos = strReturn.find("0");
		while (intPos < std::string::npos){
			strReturn.replace(intPos, 1, "$kk$");
			intPos = strReturn.find("0", intPos + 1);
		}

		intPos = strReturn.find("1");
		while (intPos < std::string::npos){
			strReturn.replace(intPos, 1, "$k$k");
			intPos = strReturn.find("1", intPos + 1);
		}
	}
	catch(...){
		throw;
	}
	return strReturn;

}

std::string DeviceSartano::getProtocol() const {
	return "sartano";
}

