#include "DeviceSartano.h"
#include "DeviceNexa.h"
#include <string>
#include <bitset>
#include <algorithm>
#include <fstream>

using namespace std;

/*
* Constructor
*/
DeviceSartano::DeviceSartano(char *strNewCode)
	:Device()
{
	strCode = strNewCode;
}


DeviceSartano::~DeviceSartano(void)
{
}


/*
* Turn on this device
*/
int DeviceSartano::turnOn(void){

	try{
		string strCode = getStringCode();
		
		strCode.insert(0, "S");
		strCode.append("$k$k$kk$$k+");	//the "turn on"-code, keeps it like this, doesn't have to be regenerated each time
		
		char* strMessage = const_cast<char*>(strCode.c_str());

		return Device::send(strMessage);
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
		string strCode = getStringCode();
		
		strCode.insert(0, "S");
		strCode.append("$kk$$k$k$k+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time
		
		char* strMessage = const_cast<char*>(strCode.c_str());

		return Device::send(strMessage);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
* Has the device got the method?
*/
int DeviceSartano::methods(char* strModel){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

/*
*	Convert an integer to byte string where 0 is represented by $k and 1 by k$, reversed and padded with 0's as needed
*/
string DeviceSartano::getStringCode(void){
	
	string strReturn = strCode;

	try{
		int intPos = (int)strReturn.find("0");
		while (intPos < string::npos){
			strReturn.replace(intPos, 1, "$kk$");
			intPos = (int)strReturn.find("0", intPos + 1);
		}

		intPos = (int)strReturn.find("1");
		while (intPos < string::npos){
			strReturn.replace(intPos, 1, "$k$k");
			intPos = (int)strReturn.find("1", intPos + 1);
		}
	}
	catch(...){
		throw;
	}
	return strReturn;

}

