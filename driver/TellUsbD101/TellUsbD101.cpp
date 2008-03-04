//DLL entry point

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

void handleException(std::exception e);
using namespace std;


#define MAX_LOADSTRING 100

//TODO:
//delete on created objects
//comment (just copy from the called methods)

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

//general settings:
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

int WINAPI devMethods(int id){

	int intMethods = 0;
	try{
		TelldusSettings ts;
		char* strModel = ts.getModel(id);
		Device* dev = ts.getDevice(id, -1); //We send dongleIndex=-1 sice we wouldn't send any data!
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

