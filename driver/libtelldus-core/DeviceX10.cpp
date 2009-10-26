#include "DeviceX10.h"
#include <string.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>

using namespace TelldusCore;

const char S = 59, L = 169;
const char B0[] = {S,S,0};
const char B1[] = {S,L,0};
const int HOUSES[] = {6,0xE,2,0xA,1,9,5,0xD,7,0xF,3,0xB,0,8,4,0xC};
const char START_CODE[] = {255,1,255,1,255,1,100,255,1,180,0};
const char STOP_CODE[] = {S,0};

/*
* Constructor
*/
DeviceX10::DeviceX10(int id, const std::string &model, const std::string &name)
	:Device(id, model, name),
	intHouse(0),
	intCode(0)
{
}

/*
* Destructor
*/
DeviceX10::~DeviceX10(void)
{}

bool DeviceX10::setHouse(const std::string &newHouse) {
	int house = 0;
	if (newHouse.length() > 0) {
		house = newHouse[0] - 'A';
	}
	if (house > 15) {
		house = 15;
	} else if (house < 0) {
		house = 0;
	}
	intHouse = HOUSES[house];
	return true;
}

bool DeviceX10::setUnit(const std::string &newUnit) {
	if (newUnit.length() > 0) {
		intCode = atoi(newUnit.c_str()) - 1;
	} else {
		intCode = 0;
	}
	return true;
}

bool DeviceX10::setDeviceParameter(const std::string &strName, const std::string &strValue) {
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
int DeviceX10::turnOn(void){

	try{
		std::string strCode = getStringCode(true);

		strCode.insert(0, "S");
		strCode.append("+");	

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
int DeviceX10::turnOff(void){

	try{
		std::string strCode = getStringCode(false);

		strCode.insert(0, "S");
		strCode.append("+");	

		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

std::string DeviceX10::getStringCode(bool on){

	std::string strReturn = START_CODE;
	std::string strComplement = "";

	try{
		int house = intHouse;
		for( int i = 0; i < 4; ++i ) {
			if (house & 1) {
				strReturn.append(B1);
				strComplement.append(B0);
			} else {
				strReturn.append(B0);
				strComplement.append(B1);
			}
			house >>= 1;
		}
		strReturn.append( B0 );
		strComplement.append( B1 );
		
		if (intCode >= 8) {
			strReturn.append(B1);
			strComplement.append(B0);
		} else {
			strReturn.append(B0);
			strComplement.append(B1);
		}
		
		strReturn.append( B0 );
		strComplement.append( B1 );
		strReturn.append( B0 );
		strComplement.append( B1 );
		
		strReturn.append( strComplement );
		strComplement = "";
		
		strReturn.append( B0 );
		strComplement.append( B1 );

		if (intCode >> 2 & 1) { //Bit 2 of intCode
			strReturn.append(B1);
			strComplement.append(B0);
		} else {
			strReturn.append(B0);
			strComplement.append(B1);
		}
		
		if (on) {
			strReturn.append(B0);
			strComplement.append(B1);
		} else {
			strReturn.append(B1);
			strComplement.append(B0);
		}
		
		if (intCode & 1) { //Bit 0 of intCode
			strReturn.append(B1);
			strComplement.append(B0);
		} else {
			strReturn.append(B0);
			strComplement.append(B1);
		}
		
		if (intCode >> 1 & 1) { //Bit 1 of intCode
			strReturn.append(B1);
			strComplement.append(B0);
		} else {
			strReturn.append(B0);
			strComplement.append(B1);
		}

		for( int i = 0; i < 3; ++i ) {
			strReturn.append( B0 );
			strComplement.append( B1 );
		}

		strReturn.append( strComplement );
		strReturn.append( STOP_CODE );
	}
	catch(...){
		throw;
	}
	return strReturn;

}

bool DeviceX10::parameterMatches( const std::string &name, const std::string &value ) const {
	return false;
}

/*
* Has the device got the method?
*/
int DeviceX10::methods(){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

std::string DeviceX10::getProtocol() const {
	return "x10";
}
