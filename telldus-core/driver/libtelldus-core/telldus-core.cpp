/**
 * @defgroup core telldus-core
 * Telldus Core is the base module used to interface a Telldus TellStick.
 * @{
 */

#ifdef _WINDOWS
	#include "stdafx.h"
	#include <ole2.h>
#endif
#include "telldus-core.h"
#include "Manager.h"
#include "Device.h"
#include "DeviceGroup.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

void handleException(std::exception e);
using namespace std;
using namespace TelldusCore;

inline char *wrapStdString( const std::string &string);

/**
 * @def TELLSTICK_TURNON
 * Device-flag for devices supporting the tdTurnOn() call.
 * 
 * @def TELLSTICK_TURNOFF
 * Device-flag for devices supporting the tdTurnOff() call.
 * 
 * @def TELLSTICK_BELL
 * Device-flag for devices supporting the tdBell() call.
 * 
 * @def TELLSTICK_TOGGLE
 * This method is currently unimplemented.
 * 
 * @def TELLSTICK_DIM
 * Device-flag for devices supporting the tdDim() call.
 * 
 * @def TELLSTICK_TYPE_DEVICE
 * Device type of a single device.
 * 
 * @def TELLSTICK_TYPE_GROUP
 * Device type of a device which contains other devices.
 * 
 * @def TELLSTICK_SUCCESS
 * Error code. Returned when the command succeeded.
 * 
 * @def TELLSTICK_ERROR_NOT_FOUND
 * Error code. Returned if a TellStick was not found on the system.
 * 
 * @def TELLSTICK_ERROR_PERMISSION_DENIED
 * Error code. Returned if the user doesn't have privileges to open
 * the TellStick device.
 * 
 * @def TELLSTICK_ERROR_DEVICE_NOT_FOUND
 * Error code. The supplied device id was not found.
 * 
 * @def TELLSTICK_ERROR_METHOD_NOT_SUPPORTED
 * Error code. The requested method is not supported device.
 * This should be avoided by a call to tdMethods().
 * 
 * @def TELLSTICK_ERROR_UNKNOWN
 * Error code. An unkown error has occurred.
 */

#define MAX_LOADSTRING 100

/**
 * Close the library and clean up the cache it uses.
 * This should be called when the library is not supposed to be used anymore
 **/
void WINAPI tdClose(void) {
	Manager::close();
}

/**
 * Turns a device on.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to turn on.
 **/
int WINAPI tdTurnOn(int intDeviceId){

	try{
		Manager *manager = Manager::getInstance();
		Device* dev = manager->getDevice(intDeviceId);
		if(dev != NULL){
			int model = dev->getModel();
			int methods = dev->methods( TELLSTICK_TURNON );
			
			int retval = 0;
			
			if ( !(methods & TELLSTICK_TURNON) ) {
				retval = TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
			} else {
				retval = dev->turnOn();
			}

			return retval;
		} else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	catch(exception e){
		handleException(e);
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/**
 * Turns a device off.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to turn off.
 */
int WINAPI tdTurnOff(int intDeviceId){

	try{
		Manager *manager = Manager::getInstance();
		Device* dev = manager->getDevice(intDeviceId);
		if(dev != NULL){
			int model = dev->getModel( );
			int methods = dev->methods( TELLSTICK_TURNOFF );
			int retval = 0;

			if ( !(methods & TELLSTICK_TURNOFF) ) {
				retval = TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
			} else {
				retval = dev->turnOff();
			}

			return retval;
		}
		else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	catch(exception e){
		handleException(e);
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/**
 * Sends bell command to devices supporting this.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to send bell to
 */
int WINAPI tdBell(int intDeviceId){

	try{
		Manager *manager = Manager::getInstance();
		Device* dev = manager->getDevice(intDeviceId);
		if(dev != NULL){
			int methods = dev->methods( TELLSTICK_BELL );
			int retval = 0;

			if ( !(methods & TELLSTICK_BELL) ) {
				retval = TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
			} else {
				retval = dev->bell();
			}

			return retval;
		}
		else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	catch(exception e){
		handleException(e);
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/**
 * Dims a device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to dim
 * @param level The level the device should dim to. This value should be 0-255
 */
int WINAPI tdDim(int intDeviceId, unsigned char level){
	try{
		Manager *manager = Manager::getInstance();
		Device* dev = manager->getDevice(intDeviceId);
		if(dev != NULL){
			int methods = dev->methods( TELLSTICK_DIM );
			int retval = 0;

			if ( !(methods & TELLSTICK_DIM) ) {
				retval = TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
			} else {
				if (level == 0) {
					retval = dev->turnOff();
				} else if (level == 255) {
					retval = dev->turnOn();
				} else {
					retval = dev->dim(level);
				}
			}

			return retval;
		}
		else{
			return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
		}
	}
	catch(exception e){
		handleException(e);
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/**
 * This function returns the number of devices configured
 * @returns an integer of the total number of devices configured
 */
int WINAPI tdGetNumberOfDevices(void){
	int intReturn = -1;
	try{
		Settings ts;
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
 * int intNumberOfDevices = tdGetNumberOfDevices();
 * for (int i = 0; i < intNumberOfDevices; i++) {
 *   int id = tdGetDeviceId( i );
 *   // id now contains the id number of the device with index of i
 * }
 * \endcode
 * @param intDeviceIndex The device index to query. The index starts from 0.
 * @returns the unique id for the device or -1 if the device is not found.
 */
int WINAPI tdGetDeviceId(int intDeviceIndex){
	int intReturn = -1;
	try{
		Settings ts;
		intReturn = ts.getDeviceId(intDeviceIndex);
	}
	catch(exception e){
		intReturn = -1;
		handleException(e);
	}
	return intReturn;
}

/**
 * Returns which type the device is. The device could be either
 * TELLSTICK_TYPE_DEVICE or TELLSTICK_TYPE_GROUP
 */
int WINAPI tdGetDeviceType(int intDeviceId) {
	try{
		Manager *manager = Manager::getInstance();
		Device* dev = manager->getDevice(intDeviceId);
		if (dev != NULL) {
			DeviceGroup *deviceGroup = dynamic_cast<DeviceGroup*>(dev);
			if (deviceGroup) {
				return TELLSTICK_TYPE_GROUP;
			}
		}
	}
	catch(exception e){
		handleException(e);
	}
	return TELLSTICK_TYPE_DEVICE;
}

/**
 * Query a device for it's name.
 * @param intDeviceId The unique id of the device to query
 * @returns The name of the device or an empty string if the device is not found.
 */
char * WINAPI tdGetName(int intDeviceId){
	std::string strReturn;
	try{
		Settings ts;
		strReturn = ts.getName(intDeviceId);
	}
	catch(exception e){
		strReturn = "";
		handleException(e);
	}
	return wrapStdString(strReturn);
}

bool WINAPI tdSetName(int intDeviceId, const char* strNewName){
	bool blnSuccess = false;
	try{
		Settings ts;
		blnSuccess = ts.setName(intDeviceId, strNewName);
	}
	catch(exception e){
		blnSuccess = false;
		handleException(e);
	}
	return blnSuccess;
}

char* WINAPI tdGetProtocol(int intDeviceId){
	std::string strReturn = "";
	try{
		Manager *manager = Manager::getInstance();
		Device* dev = manager->getDevice(intDeviceId);
		if (dev != NULL) {
			strReturn = dev->getProtocol();
		}
	}
	catch(exception e){
		handleException(e);
	}
	return wrapStdString(strReturn);
}

bool WINAPI tdSetProtocol(int intDeviceId, const char* strProtocol){
	bool blnSuccess = false;
	try{
		Manager *manager = Manager::getInstance();
		blnSuccess = manager->setProtocol(intDeviceId, strProtocol);
	}
	catch(exception e){
		blnSuccess = false;
		handleException(e);
	}
	return blnSuccess;
}

int WINAPI tdGetModel(int intDeviceId){
	int intReturn = 0;
	try{
		Manager *manager = Manager::getInstance();
		Device* dev = manager->getDevice(intDeviceId);
		if (dev != NULL) {
			intReturn = dev->getModel();
		}
	}
	catch(exception e){
		intReturn = 0;
		handleException(e);
	}
	return intReturn;
}

bool WINAPI tdSetModel(int intDeviceId, int intModel){
	bool blnSuccess = false;
	try{
		Manager *manager = Manager::getInstance();
		manager->setModel(intDeviceId, intModel);
	}
	catch(exception e){
		blnSuccess = false;
		handleException(e);
	}
	return blnSuccess;
}

bool WINAPI tdSetDeviceParameter(int intDeviceId, const char *strName, const char *strValue){

	try{
		Settings ts;
		return ts.setDeviceParameter(intDeviceId, strName, strValue);
	}
	catch(exception e){
		handleException(e);
	}
	return false;
}

char * WINAPI tdGetDeviceParameter(int intDeviceId, const char *strName, const char *defaultValue){
	std::string strReturn = "";
	try{
		Settings ts;
		strReturn = ts.getDeviceParameter(intDeviceId, strName);
		if (strReturn.empty()) {
			strReturn = defaultValue;
		}
	}
	catch(exception e){
		handleException(e);
	}
	return wrapStdString(strReturn);
}

int WINAPI tdAddDevice(){
	int intNewDeviceId = -1;
	try{
		Settings ts;
		intNewDeviceId = ts.addDevice();
	}
	catch(exception e){
		intNewDeviceId = -1;
		handleException(e);
	}
	return intNewDeviceId;
}

bool WINAPI tdRemoveDevice(int intDeviceId){
	bool blnSuccess = false;
	try{
		Settings ts;
		blnSuccess = ts.removeDevice(intDeviceId);
	}
	catch(exception e){
		blnSuccess = false;
		handleException(e);
	}
	return blnSuccess;
}

/**
 * Query a device for which methods it supports. By supplying the methods you support
 * the library could remap the methods a device support for better fit the application.
 * Example of querying a device supporting TELLSTICK_BELL:
 * \code
 * int methods = tdMethods(id, TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_BELL);
 * //methods is now TELLSTICK_BELL
 * int methods = tdMethods(idm TELLSTICK_TURNON | TELLSTICK_TURNOFF);
 * //methods is now TELLSTICK_TURNON because the client application doesn't support TELLSTICK_BELL
 * \endcode
 * @param id The device id to query
 * @param methodsSupported The methods the client application supports
 * @returns The method-flags OR'ed into an integer.
 * @sa TELLSTICK_TURNON
 * @sa TELLSTICK_TURNOFF
 * @sa TELLSTICK_BELL
 * @sa TELLSTICK_TOGGLE
 * @sa TELLSTICK_DIM
 */
int WINAPI tdMethods(int id, int methodsSupported){

	int intMethods = 0;
	try{
		Manager *manager = Manager::getInstance();
		Device* dev = manager->getDevice(id);
		if (dev != NULL) {
			intMethods = dev->methods(methodsSupported);
		}
	}
	catch(exception e){
		intMethods = 0;
		handleException(e);
	}
	intMethods = intMethods & methodsSupported; //Strip the methods not supported by client.
	return intMethods;
}

/**
 * Get a human readable string from an error code returned
 * from a function in telldus-core.
 * @param intErrorNo The error code to translate.
 * @returns a string ready to show to the user.
 * @sa TELLSTICK_SUCCESS
 * @sa TELLSTICK_ERROR_NOT_FOUND
 * @sa TELLSTICK_ERROR_PERMISSION_DENIED
 * @sa TELLSTICK_ERROR_DEVICE_NOT_FOUND
 * @sa TELLSTICK_ERROR_METHOD_NOT_SUPPORTED
 * @sa TELLSTICK_ERROR_UNKNOWN
 */
char * WINAPI tdGetErrorString(int intErrorNo) {
	const int numResponses = 5;
	const char *responses[numResponses] = {
		"Success",
		"TellStick not found",
		"Permission denied",
		"Device not found",
		"The method you tried to use is not supported by the device"
	};
	std::string strReturn;
	intErrorNo = abs(intErrorNo); //We don't use negative values here.
	if (intErrorNo >= numResponses) {
		strReturn = "Unknown error";
	} else {
		// Copy the error string to strReturn
		strReturn = responses[intErrorNo];
	}
	return wrapStdString(strReturn);
}


//********
//* Error management, set strLogName to "" to turn off
//*
void handleException(exception e){

// 	std::string strLogName = "errorlog.txt";
	std::string strLogName = "";

	if(strLogName.length() > 0){
		ofstream errorfile(strLogName.c_str(), ios::app);
		if(errorfile){
			errorfile << e.what() << endl;
			errorfile.close();
		}
	}
}

inline char *wrapStdString( const std::string &string) {
	char *returnVal;
#ifdef _WINDOWS
	returnVal = (char *)SysAllocStringByteLen(string.c_str(), string.size());
#else
	returnVal = (char *)malloc(sizeof(char) * (string.size()+1));
	strcpy(returnVal, string.c_str());
#endif
	return returnVal;
}

/*\@}*/
