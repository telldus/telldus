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
DeviceWaveman::DeviceWaveman(int id, int model, const std::string &strHouse, const std::string &strCode)
	:DeviceNexa(id, model, strHouse, strCode){
}

/*
* Turn off this device
*/
int DeviceWaveman::turnOff(void){
	
	try{
		std::string strCode = getStringCode(intHouse);
		std::string strUnit = getStringCode(intCode);
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

/*
* Has the device got the method?
*/
int DeviceWaveman::methods(int){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}
