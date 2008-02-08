// #include "StdAfx.h" //Needed?
#include "DeviceIkea.h"
#include <string>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>

using namespace std;

/*
* Constructor
*/
DeviceIkea::DeviceIkea(int intNewSystem, int intNewUnits, int intNewFadeStyle, int intDeviceIndex):Device(intDeviceIndex){
	intSystem = intNewSystem;
	intUnits = intNewUnits;
	intFadeStyle = intNewFadeStyle;
}

/*
* Destructor
*/
DeviceIkea::~DeviceIkea(void)
{
	intSystem = -1;
	intUnits = -1;
	intFadeStyle = -1;
}

/*
* Turn on this device
*/
void DeviceIkea::turnOn(void){
	try{
		string strCode = getStringCode(255);

		char* strMessage = const_cast<char*>(strCode.c_str());

		Device::send(strMessage);
	}
	catch(...){
		throw;
	}
}

/*
* Turn off this device
*/
void DeviceIkea::turnOff(void){
	try{
		string strCode = getStringCode(0);

		char* strMessage = const_cast<char*>(strCode.c_str());

		Device::send(strMessage);
	}
	catch(...){
		throw;
	}
}

/*
* Turn off this device
*/
void DeviceIkea::dim(unsigned char level){
	try{
		string strCode = getStringCode(level);

		char* strMessage = const_cast<char*>(strCode.c_str());

		Device::send(strMessage);
	}
	catch(...){
		throw;
	}
}

/*
*	Convert an integer to byte string where 0 is represented by ª and 1 by TT
*/
string DeviceIkea::getStringCode(unsigned char level){
	
	string strReturn = "STTTTTTª"; //Startcode, always like this;

	try{
		string strChannels = "";
		int intCode = (intSystem << 10) | intUnits;
		int checksum1 = 0;
		int checksum2 = 0;
		for (int i = 13; i >= 0; --i) {
			if ((intCode>>i) & 1) {
				strChannels.append("TT");
				if (13 % 2 == 0)
					checksum2++;
				else
					checksum1++;
			} else {
				strChannels.append("ª");
			}
		}
		strReturn.append(strChannels); //System + Units

		strReturn.append(checksum1 %2 == 0 ? "TT" : "ª"); //1st checksum
		strReturn.append(checksum2 %2 == 0 ? "TT" : "ª"); //2nd checksum

		if (level <= 12) {
			strReturn.append("ªTTªTT"); //Level 0 - Off
		} else if (level <= 137) {
			strReturn.append("TTªTTª"); //Level 5
		} else if (level <= 162) {
			strReturn.append("ªTTTTª"); //Level 6
		} else {
			strReturn.append("ªªªª"); //Level 10 - On
		}

		if (intFadeStyle == 1)
			strReturn.append("TTTTªª"); //Smooth
		else
			strReturn.append("TTªªTT"); //Instant
		strReturn.append("+");

	}
	catch(...){
		throw;
	}
	return strReturn;

}

/*
* Has the device got the method?
*/
int DeviceIkea::methods(char* strModel){

	if(strcmp(strModel, TELLSTICK_DEVICE_KOPPLA) == 0) {
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM);
	}
	return 0;
}
