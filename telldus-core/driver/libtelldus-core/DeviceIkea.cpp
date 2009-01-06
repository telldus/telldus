// #include "StdAfx.h" //Needed?
#include "DeviceIkea.h"
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

#ifdef _WINDOWS
#define strcasecmp(x, y) _strcmpi(x, y)
#endif

/*
* Constructor
*/
DeviceIkea::DeviceIkea(char *strSystem, char *strUnits, char *strFadeStyle)
	:Device()
{
	if (strSystem != NULL && strlen(strSystem) > 0) {
		intSystem = atoi(strSystem) - 1;
	} else {
		intSystem = 0;
	}
	if (strUnits != NULL && strlen(strUnits) > 0) {
		intUnits = 0; //Start without any units

		char *strTemp = strtok(strUnits, ",");
		do {
			int intUnit = atoi(strTemp);
			if (intUnit == 10) {
				intUnit = 0;
			}
			intUnits = intUnits | ( 1<<(9-intUnit) );
		} while ( (strTemp = strtok(NULL, ",")) != NULL );
	}
	if (strUnits != NULL && strlen(strUnits) > 0 && strcasecmp(strFadeStyle, "true") == 0) {
		intFadeStyle = 1;
	} else {
		intFadeStyle = 0;
	}
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
int DeviceIkea::turnOn(void){
	try{
		string strCode = getStringCode(255);

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
int DeviceIkea::turnOff(void){
	try{
		string strCode = getStringCode(0);

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
int DeviceIkea::dim(unsigned char level){
	try{
		string strCode = getStringCode(level);

		char* strMessage = const_cast<char*>(strCode.c_str());

		return Device::send(strMessage);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
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
				if (i % 2 == 0)
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

		int intLevel = 0;
		if (level <= 12) {
			intLevel = 10; // Level 10 is actually off
		} else if (level <= 37) {
			intLevel = 1;
		} else if (level <= 62) {
			intLevel = 2;
		} else if (level <= 87) {
			intLevel = 3;
		} else if (level <= 112) {
			intLevel = 4;
		} else if (level <= 137) {
			intLevel = 5;
		} else if (level <= 162) {
			intLevel = 6;
		} else if (level <= 187) {
			intLevel = 7;
		} else if (level <= 212) {
			intLevel = 8;
		} else if (level <= 237) {
			intLevel = 9;
		} else {
			intLevel = 0; // Level 0 is actually full on
		}

		int intFade = 0;
		if (intFadeStyle == 1) {
			intFade = 11 << 4; //Smooth
		} else {
			intFade = 1 << 4; //Instant
		}

		intCode = intLevel | intFade; //Concat level and fade

		checksum1 = 0;
		checksum2 = 0;
		for (int i = 0; i < 6; ++i) {
			if ((intCode>>i) & 1) {
				strReturn.append("TT");
				if (i % 2 == 0)
					checksum1++;
				else
					checksum2++;
			} else {
				strReturn.append("ª");
			}
		}

		strReturn.append(checksum1 %2 == 0 ? "TT" : "ª"); //1st checksum
		strReturn.append(checksum2 %2 == 0 ? "TT" : "ª"); //2nd checksum

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
int DeviceIkea::methods(int){

	if(this->getModel() == TELLSTICK_DEVICE_KOPPLA) {
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM);
	}
	return 0;
}

std::string DeviceIkea::getProtocol() {
	return "ikea";
}
