// #include "StdAfx.h"
#include "DeviceNexa.h"
#include <string.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>

using namespace std;

/*
* Constructor
*/
DeviceNexa::DeviceNexa(char *strNewHouse, char *strNewCode)
	:Device()
{
	if (strNewHouse != NULL && strlen(strNewHouse) > 0) {
		intHouse = strNewHouse[0] - 'A';
	} else {
		intHouse = 0;
	}

	if (strNewCode != NULL && strlen(strNewCode) > 0) {
		intCode = atoi(strNewCode) - 1;
	} else {
		intCode = 0;
	}
}

/*
* Destructor
*/
DeviceNexa::~DeviceNexa(void)
{
// 	intHouse = -1;
// 	intCode = -1;
}

/*
* Turn on this device
*/
int DeviceNexa::turnOn(void){

	try{
		string strCode = getStringCode(intHouse);
		string strUnit = getStringCode(intCode);
		strCode.append(strUnit);

		strCode.insert(0, "S");
		strCode.append("$k$k$kk$$kk$$kk$$k+");	//the "turn on"-code, keeps it like this, doesn't have to be regenerated each time

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
int DeviceNexa::turnOff(void){

	try{
		string strCode = getStringCode(intHouse);
		string strUnit = getStringCode(intCode);
		strCode.append(strUnit);

		strCode.insert(0, "S");
		strCode.append("$k$k$kk$$kk$$k$k$k+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time

		char* strMessage = const_cast<char*>(strCode.c_str());

		return Device::send(strMessage);
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
		string strCode = getStringCode(intHouse);

		strCode.append("$kk$$kk$$kk$$k$k"); //the unit-code is always 7, doesn't have to be regenerated each time
		strCode.insert(0, "S");
		strCode.append("$kk$$kk$$kk$$kk$$k+");	//the "bell"-code, keeps it like this, doesn't have to be regenerated each time

		char* strMessage = const_cast<char*>(strCode.c_str());

		return Device::send(strMessage);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
*	Convert an integer to byte string where 0 is represented by $k and 1 by k$, reversed and padded with 0's as needed
*/
string DeviceNexa::getStringCode(int intToConvert){

	string strReturn = "";

	try{
		bitset<4> bs ((long)intToConvert);

		strReturn = bs.to_string();
		reverse(strReturn.begin(), strReturn.end());

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

/*
* Has the device got the method?
*/
int DeviceNexa::methods(int intModel){

	if( (intModel == TELLSTICK_DEVICE_YCR3500) ||
		(intModel == TELLSTICK_DEVICE_YCR300D) ||
		(intModel == TELLSTICK_DEVICE_WSR1000) ||
		(intModel == TELLSTICK_DEVICE_CMR1000) ||
		(intModel == TELLSTICK_DEVICE_CMR300) ||
		(intModel == TELLSTICK_DEVICE_PA33300) ||
		(intModel == TELLSTICK_DEVICE_EL2000) ||
		(intModel == TELLSTICK_DEVICE_EL2005) ||
		(intModel == TELLSTICK_DEVICE_EL2006) ||
		(intModel == TELLSTICK_DEVICE_SYCR3500) ||
		(intModel == TELLSTICK_DEVICE_SYCR300) ||
		(intModel == TELLSTICK_DEVICE_HDR105) ||
		(intModel == TELLSTICK_DEVICE_EL2004) ||
		(intModel == TELLSTICK_DEVICE_EL2016) ||
		(intModel == TELLSTICK_DEVICE_EL2010) ||
		(intModel == TELLSTICK_DEVICE_LYCR1000) ||
		(intModel == TELLSTICK_DEVICE_LYCR300) ||
		(intModel == TELLSTICK_DEVICE_LCMR1000) ||
		(intModel == TELLSTICK_DEVICE_LCMR300) ||
		(intModel == TELLSTICK_DEVICE_EL2023) ||
		(intModel == TELLSTICK_DEVICE_EL2024) ||
		(intModel == TELLSTICK_DEVICE_EL2021) ||
		(intModel == TELLSTICK_DEVICE_EL2017) ||
		(intModel == TELLSTICK_DEVICE_EL2019)
 		)
	{
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
	} else if ( intModel == TELLSTICK_DEVICE_ML7100) {
		return TELLSTICK_BELL;
	}
	return 0;
}
