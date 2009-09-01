// #include "StdAfx.h"
#include "DeviceNexa.h"
#include <string.h>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>
#include "common.h"

using namespace TelldusCore;

const char START[] = {'T',127,255,24,1,0};

/*
* Constructor
*/
DeviceNexa::DeviceNexa(int id, const std::string &model, const std::string &name)
	:Device(id, model, name),
	intHouse(0),
	intCode(0)
{
}

/*
* Destructor
*/
DeviceNexa::~DeviceNexa(void)
{}

bool DeviceNexa::setHouse(const std::string &newHouse) {
	int house = atoi(newHouse.c_str());
	if (newHouse.length() == 1 && house == 0) { //Codeswitch
		intHouse = newHouse[0] - 'A';
	} else { //Selflearning
		intHouse = house;
	}
	return true;
}

bool DeviceNexa::setUnit(const std::string &newUnit) {
	if (newUnit.length() > 0) {
		intCode = atoi(newUnit.c_str()) - 1;
	} else {
		intCode = 0;
	}
	return true;
}

bool DeviceNexa::setDeviceParameter(const std::string &strName, const std::string &strValue) {
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
int DeviceNexa::turnOn(void){
	if (strcasecmp(this->getModel().c_str(), "bell") == 0) {
		return bell();
	}
	try{
		std::string strCode = "";
		if (isDimmer()) {
			return dim(255);
		} else if (isSelflearning()) {
			strCode.append( 1, 'R' );
			strCode.append( 1, 5 );
			strCode.append(getStringSelflearning(false, 255));
		} else {
			strCode.append("S");
			strCode.append(getStringCodeSwitch(intHouse));
			strCode.append(getStringCodeSwitch(intCode));
			strCode.append("$k$k$kk$$kk$$kk$$k+");	//the "turn on"-code, keeps it like this, doesn't have to be regenerated each time
		}

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
int DeviceNexa::turnOff(void){

	try{
		std::string strCode = "";
		if (isSelflearning()) {
			strCode.append( 1, 'R' );
			strCode.append( 1, 5 );
			strCode.append(getStringSelflearning(false, 0));
		} else {
			strCode.append("S");
			strCode.append(getStringCodeSwitch(intHouse));
			strCode.append(getStringCodeSwitch(intCode));
			strCode.append("$k$k$kk$$kk$$k$k$k+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time
		}

		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
* Dim this device
*/
int DeviceNexa::dim(unsigned char level) {
	try{
		std::string strMessage = "";
		strMessage.append( 1, 'R' );
		strMessage.append( 1, 5 );
		strMessage.append(getStringSelflearning(true, level));
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
		std::string strCode = getStringCodeSwitch(intHouse);

		strCode.append("$kk$$kk$$kk$$k$k"); //the unit-code is always 7, doesn't have to be regenerated each time
		strCode.insert(0, "S");
		strCode.append("$kk$$kk$$kk$$kk$$k+");	//the "bell"-code, keeps it like this, doesn't have to be regenerated each time

		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

int DeviceNexa::learn(void){
	std::string strCode = "";
	strCode.append( 1, 'R' );
	strCode.append( 1, 2 );
	strCode.append(getStringSelflearning(false, 255));
	
	int retVal = 0;
	for (int i = 0; i < 5; ++i) {
		retVal = Device::send(strCode);
		if (retVal != TELLSTICK_SUCCESS) {
			return retVal;
		}
		msleep(200);
	}
	return retVal;
}

/*
*	Convert an integer to byte string where 0 is represented by $k and 1 by k$, reversed and padded with 0's as needed
*/
std::string DeviceNexa::getStringCodeSwitch(int intToConvert){

	std::string strReturn = "";

	try{
		for( int i = 0; i < 4; ++i ) {
			if (intToConvert & 1) { //Convert 1
				strReturn.append("$kk$");
			} else { //Convert 0
				strReturn.append("$k$k");
			}
			intToConvert >>= 1;
			
		}
	}
	catch(...){
		throw;
	}
	return strReturn;

}

std::string DeviceNexa::getStringSelflearning(bool dim, unsigned char level) {
	std::string strMessage = START;
	strMessage.append(1,(dim ? 147 : 132)); //Number of pulses
		
	std::string m;
	for (int i = 25; i >= 0; --i) {
		m.append( intHouse & 1 << i ? "10" : "01" );
	}
	m.append("01"); //Group
	
	//On/off
	if (dim) {
		m.append("00");
	} else if (level == 0) {
		m.append("01");
	} else {
		m.append("10");
	}
	
	for (int i = 3; i >= 0; --i) {
		m.append( intCode & 1 << i ? "10" : "01" );
	}
	
	if (dim) {
		unsigned char newLevel = level/16;
		for (int i = 3; i >= 0; --i) {
			m.append(newLevel & 1 << i ? "10" : "01");
		}
	}
	
	//The number of data is odd.
	//Add this to make it even, otherwise the following loop will not work
	m.append("0"); 
	
	unsigned char code = 9; //b1001, startcode
	for (unsigned int i = 0; i < m.length(); ++i) {
		code <<= 4;
		int v = 0;
		if (m[i] == '1') {
			code |= 8; //b1000
		} else {
			code |= 10; //b1010;
		}
		if (i % 2 == 0) {
			strMessage.append(1,code);
			code = 0;
		}
	}
	strMessage.append("+");
	
// 	for( int i = 0; i < strMessage.length(); ++i ) {
// 		printf("%i,", (unsigned char)strMessage[i]);
// 	}
// 	printf("\n");
	return strMessage;
}

bool DeviceNexa::parameterMatches( const std::string &name, const std::string &value ) const {
	if (value.length() == 0) {
		return false;
	}
	
	if (name.compare("arctech_house") == 0) {
		return intHouse == value[0] - 'A';
	} else if (name.compare("arctech_unit") == 0) {
		return intCode == atoi(value.c_str()) - 1;
	}
	return true;
}

/*
* Has the device got the method?
*/
int DeviceNexa::methods(){
	std::string strModel = this->getModel();
	
	if ( strcasecmp(strModel.c_str(), "codeswitch") == 0 ) {
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
		
	} else if (strcasecmp(strModel.c_str(), "selflearning-switch") == 0) {
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_LEARN);
		
	} else if (strcasecmp(strModel.c_str(), "selflearning-dimmer") == 0) {
		return (TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM | TELLSTICK_LEARN);
		
	} else if (strcasecmp(strModel.c_str(), "bell") == 0) {
		return TELLSTICK_BELL;
	}
	return 0;
}

bool DeviceNexa::isSelflearning() const {
	return strncasecmp(getModel().c_str(), "selflearning", 12) == 0;
}

bool DeviceNexa::isDimmer() const {
	return strcasecmp(getModel().c_str(), "selflearning-dimmer") == 0;
}

std::string DeviceNexa::getProtocol() const {
	return "arctech";
}
