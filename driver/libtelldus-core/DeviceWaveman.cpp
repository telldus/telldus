// #include "StdAfx.h"
#include "DeviceWaveman.h"
#include <string>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>

using namespace TelldusCore;

/*
* Constructor
*/
DeviceWaveman::DeviceWaveman(int id, const std::string &model, const std::string &name)
	:DeviceNexa(id, model, name){
}

/*
* Turn off this device
*/
int DeviceWaveman::turnOff(void){
	
	try{
		std::string strCode = getStringCodeSwitch(intHouse);
		std::string strUnit = getStringCodeSwitch(intCode);
		strCode.append(strUnit);
		
		strCode.insert(0, "S");
		strCode.append("$k$k$k$k$k$k$k$k$k+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time
		
		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

std::string DeviceWaveman::getStringCodeSwitch(int intToConvert){

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

/*
* Has the device got the method?
*/
int DeviceWaveman::methods(){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

std::string DeviceWaveman::getProtocol() const {
	return "waveman";
}
