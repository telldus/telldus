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
DeviceSartano::DeviceSartano(int intNewSystem, int intNewCode, int intDeviceIndex)
	:Device(intDeviceIndex)
{
	intSystem = intNewSystem;
	intCode = intNewCode;
}


DeviceSartano::~DeviceSartano(void)
{
	intSystem = -1;
	intCode = -1;
}


/*
* Turn on this device
*/
void DeviceSartano::turnOn(void){

	try{
		string strSystem = getStringCode(intSystem);
		string strCode = getStringCode(intCode);
		strSystem.append(strCode);
		
		strSystem.insert(0, "S");
		strSystem.append("$k$k$kk$$k+");	//the "turn on"-code, keeps it like this, doesn't have to be regenerated each time
		
		char* strMessage = const_cast<char*>(strSystem.c_str());

		Device::send(strMessage);
	}
	catch(...){
		throw;
	}
}

/*
* Turn off this device
*/
void DeviceSartano::turnOff(void){
	
	try{
		string strSystem = getStringCode(intSystem);
		string strCode = getStringCode(intCode);
		strSystem.append(strCode);
		
		strSystem.insert(0, "S");
		strSystem.append("$kk$$k$k$k+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time
		
		char* strMessage = const_cast<char*>(strSystem.c_str());

		Device::send(strMessage);
	}
	catch(...){
		throw;
	}
}

/*
* Has the device got the method?
*/
bool DeviceSartano::hasMethod(int methodname, char* strModel){

	bool blnExists = false;

	try{
//		if(strModel == "xxxx" || strModel == "yyyy"){
			if(methodname == TELLSTICK_TURNON || methodname == TELLSTICK_TURNOFF){
				blnExists = true;
//			}
		}
	}
	catch(...){
		throw;
	}
	return blnExists;
}

/*
*	Convert an integer to byte string where 0 is represented by $k and 1 by k$, reversed and padded with 0's as needed
*/
string DeviceSartano::getStringCode(int intToConvert){
	
	string strReturn = "";

	try{
		bitset<5> bs ((long)intToConvert);
		
		strReturn = bs.to_string();

		int intPos = (int)strReturn.find("0");
		while (intPos < string::npos){
			strReturn.replace(intPos, 1, "$k");
			intPos = (int)strReturn.find("0", intPos + 1);
		}

		intPos = (int)strReturn.find("1");
		while (intPos < string::npos){
			strReturn.replace(intPos, 1, "k$");
			intPos = (int)strReturn.find("1", intPos + 1);
		}
	
		intPos = 0;
		while (intPos < (int)strReturn.length()){
			strReturn.insert(intPos, "$k");
			intPos = intPos + 4;
		}
	}
	catch(...){
		throw;
	}
	return strReturn;

}

