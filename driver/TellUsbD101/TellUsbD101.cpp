/**
 * @defgroup core telldus-core
 * Telldus Core is the base module used to interface a Telldus TellStick.
 */

#ifdef _WINDOWS
	#include "stdafx.h"
	#include <ole2.h>
#endif
#include "TellUsbD101.h"
#include "settings/TelldusSettings.h"
#include "Device.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>

void handleException(std::exception e);
using namespace std;

/**
 * @def TELLSTICK_TURNON
 * @ingroup core
 * Device-flag for devices supporting the devTurnOn() call.
 * 
 * @def TELLSTICK_TURNOFF
 * @ingroup core
 * Device-flag for devices supporting the devTurnOff() call.
 * 
 * @def TELLSTICK_BELL
 * @ingroup core
 * Device-flag for devices supporting the devBell() call.
 * 
 * @def TELLSTICK_TOGGLE
 * @ingroup core
 * This method is currently unimplemented
 * 
 * @def TELLSTICK_DIM
 * @ingroup core
 * Device-flag for devices supporting the devDim() call.
 */

#define MAX_LOADSTRING 100

//TODO:
//delete on created objects
//comment (just copy from the called methods)

/**
 * Turns a device on.
 * Make sure the device supports this by calling devMethods() before any
 * call to this function.
 * @param intDeviceId The device id to turn on.
 * @ingroup core
 **/
bool WINAPI devTurnOn(int intDeviceId){

	try{
		TelldusSettings ts;
		Device* dev = ts.getDevice(intDeviceId);
		if(dev != NULL){
			dev->turnOn();

			delete(dev);
			return true;
		}
		else{
			return false;
		}
	}
	catch(exception e){
		handleException(e);
	}
	return false;
}

/**
 * Turns a device off.
 * Make sure the device supports this by calling devMethods() before any
 * call to this function.
 * @param intDeviceId The device id to turn off.
 * @ingroup core
 */
bool WINAPI devTurnOff(int intDeviceId){

	try{
		TelldusSettings ts;
		Device* dev = ts.getDevice(intDeviceId);
		if(dev != NULL){
			dev->turnOff();

			delete(dev);
			return true;
		}
		else{
			return false;
		}
	}
	catch(exception e){
		handleException(e);
	}
	return false;
}

/**
 * Sends bell command to devices supporting this.
 * Make sure the device supports this by calling devMethods() before any
 * call to this function.
 * @param intDeviceId The device id to send bell to
 * @ingroup core
 */
bool WINAPI devBell(int intDeviceId){

	try{
		TelldusSettings ts;
		Device* dev = ts.getDevice(intDeviceId);
		if(dev != NULL){
			dev->bell();

			delete(dev);
			return true;
		}
		else{
			return false;
		}
	}
	catch(exception e){
		handleException(e);
	}
	return false;
}

/**
 * Dims a device.
 * Make sure the device supports this by calling devMethods() before any
 * call to this function.
 * @param intDeviceId The device id to dim
 * @param level The level the device should dim to. This value should be 0-255
 * @ingroup core
 */
bool WINAPI devDim(int intDeviceId, unsigned char level){

	try{
		TelldusSettings ts;
		Device* dev = ts.getDevice(intDeviceId);
		if(dev != NULL){
			if (level == 0) {
				dev->turnOff();
			} else if (level == 255) {
				dev->turnOn();
			} else {
				dev->dim(level);
			}
			delete(dev);
			return true;
		}
		else{
			return false;
		}
	}
	catch(exception e){
		handleException(e);
	}
	return false;
}

/**
 * This function returns the number of devices configured
 * @returns an integer of the total number of devices configured
 * @ingroup core
 */
int WINAPI devGetNumberOfDevices(void){
	int intReturn = -1;
	try{
		TelldusSettings ts;
		intReturn = ts.getNumberOfDevices();
	}
	catch(exception e){
		intReturn = -1;
		handleException(e);
	}
	return intReturn;
}

/**
 * This function returns the unique id of a device with a specific index.
 * To get all the id numbers you should loop over all the devices:
 * \code
 * int intNumberOfDevices = devGetNumberOfDevices();
 * for (int i = 0; i < intNumberOfDevices; i++) {
 *   int id = devGetDeviceId( i );
 *   // id now contains the id number of the device with index of i
 * }
 * \endcode
 * @param intDeviceIndex The device index to query. The index starts from 0.
 * @returns the unique id for the device or -1 if the device is not found.
 * @ingroup core
 */
int WINAPI devGetDeviceId(int intDeviceIndex){
	int intReturn = -1;
	try{
		TelldusSettings ts;
		intReturn = ts.getDeviceId(intDeviceIndex);
	}
	catch(exception e){
		intReturn = -1;
		handleException(e);
	}
	return intReturn;
}

/**
 * Query a device for it's name.
 * @param intDeviceId The unique id of the device to query
 * @returns The name of the device or an empty string if the device is not found.
 * @ingroup core
 */
char * WINAPI devGetName(int intDeviceId){
	char* strReturn;
	try{
		TelldusSettings ts;
		strReturn = ts.getName(intDeviceId);
#ifdef _WINDOWS
		strReturn = (char *)SysAllocStringByteLen (strReturn, lstrlen(strReturn));
#endif
	}
	catch(exception e){
		strReturn = "";
		handleException(e);
	}
	return strReturn;
}

bool WINAPI devSetName(int intDeviceId, const char* strNewName){
	bool blnSuccess = false;
	try{
		TelldusSettings ts;
		blnSuccess = ts.setName(intDeviceId, strNewName);
	}
	catch(exception e){
		blnSuccess = false;
		handleException(e);
	}
	return blnSuccess;
}

char* WINAPI devGetVendor(int intDeviceId){
	char* strReturn = "";
	try{
		TelldusSettings ts;
		strReturn = ts.getVendor(intDeviceId);
#ifdef _WINDOWS
		strReturn = (char *)SysAllocStringByteLen (strReturn, lstrlen(strReturn));
#endif
	}
	catch(exception e){
		strReturn = "";
		handleException(e);
	}
	return strReturn;
}

bool WINAPI devSetVendor(int intDeviceId, const char* strVendor){
	bool blnSuccess = false;
	try{
		TelldusSettings ts;
		blnSuccess = ts.setVendor(intDeviceId, strVendor);
	}
	catch(exception e){
		blnSuccess = false;
		handleException(e);
	}
	return blnSuccess;
}

 char* WINAPI devGetModel(int intDeviceId){
	char* strReturn = "";
	try{
		TelldusSettings ts;
		strReturn = ts.getModel(intDeviceId);
#ifdef _WINDOWS
		strReturn = (char *)SysAllocStringByteLen (strReturn, lstrlen(strReturn));
#endif
	}
	catch(exception e){
		strReturn = "";
		handleException(e);
	}
	return strReturn;
}

bool WINAPI devSetModel(int intDeviceId, const char* strNewModel){
	bool blnSuccess = false;
	try{
		TelldusSettings ts;
		blnSuccess = ts.setModel(intDeviceId, strNewModel);
	}
	catch(exception e){
		blnSuccess = false;
		handleException(e);
	}
	return blnSuccess;
}

bool WINAPI devSetArgument(int intDeviceId, const char *strName, const char *strValue){

	try{
		TelldusSettings ts;
		return ts.setArgument(intDeviceId, strName, strValue);
	}
	catch(exception e){
		handleException(e);
	}
	return false;
}

const char * WINAPI devGetArgument(int intDeviceId, const char *strName, const char *defaultValue){
	char *strReturn = "";
	try{
		TelldusSettings ts;
		strReturn = ts.getArgument(intDeviceId, strName);
		if (strReturn == NULL) {
			return defaultValue;
		}
#ifdef _WINDOWS
		strReturn = (char *)SysAllocStringByteLen (strReturn, lstrlen(strReturn));
#endif
	}
	catch(exception e){
		handleException(e);
	}
	return strReturn;
}

int WINAPI devAddDevice(){
	int intNewDeviceId = -1;
	try{
		TelldusSettings ts;
		intNewDeviceId = ts.addDevice();
	}
	catch(exception e){
		intNewDeviceId = -1;
		handleException(e);
	}
	return intNewDeviceId;
}

bool WINAPI devRemoveDevice(int intDeviceId){
	bool blnSuccess = false;
	try{
		TelldusSettings ts;
		blnSuccess = ts.removeDevice(intDeviceId);
	}
	catch(exception e){
		blnSuccess = false;
		handleException(e);
	}
	return blnSuccess;
}

/**
 * Query a device for which methods it supports.
 * @param id The device id to query
 * @returns The method-flags OR'ed into an integer.
 * @ingroup core
 * @sa TELLSTICK_TURNON
 * @sa TELLSTICK_TURNOFF
 * @sa TELLSTICK_BELL
 * @sa TELLSTICK_TOGGLE
 * @sa TELLSTICK_DIM
 */
int WINAPI devMethods(int id){

	int intMethods = 0;
	try{
		TelldusSettings ts;
		char* strModel = ts.getModel(id);
		Device* dev = ts.getDevice(id);
		if (dev != NULL) {
			intMethods = dev->methods(strModel);
		}
	}
	catch(exception e){
		intMethods = 0;
		handleException(e);
	}
	return intMethods;
}


//********
//* Error management, set strLogName to "" to turn off
//*
void handleException(exception e){

	char* strLogName = "c:\\errorlog.txt";
	//char* strLogName = "";

	if(strlen(strLogName) > 0){
		ofstream errorfile(strLogName, ios::app);
		if(errorfile){
			errorfile << e.what() << endl;
			errorfile.close();
		}
	}
}

