//DLL entry point

#include "TellUsbD101.h"
#include "../libtelldus-core/telldus-core.h"
#include <stdio.h>
#include <windows.h>
#include <string>
#include <sstream> 


int __stdcall devGetNumberOfDevices(void){
	return tdGetNumberOfDevices();
}

int __stdcall devGetDeviceId(int intDeviceIndex){
	return tdGetDeviceId(intDeviceIndex);
}

char * __stdcall devGetName(int intDeviceId){
	return tdGetName(intDeviceId);
}

char* __stdcall devGetVendor(int intDeviceId){
	return tdGetProtocol(intDeviceId);
}

char* __stdcall devGetModel(int intDeviceId){
	return tdGetModel(intDeviceId);
}

int __stdcall devGetNumberOfArguments(int intDeviceId){
	return 0;
}

int __stdcall devGetArgument(int intDeviceId, int intArgumentIndex){
	return 0;
}

bool __stdcall devSetName(int intDeviceId, char* strNewName){
	return tdSetName(intDeviceId, strNewName);
}

bool __stdcall devSetVendor(int intDeviceId, char* strVendor) {
	if (_stricmp(strVendor, "nexa") == 0) {
		return tdSetProtocol(intDeviceId, "arctech");
	} else {
		return tdSetProtocol(intDeviceId, strVendor);
	}
}

bool __stdcall devSetModel(int intDeviceId, char* strNewModel){
	return tdSetModel(intDeviceId, strNewModel);
}

bool __stdcall devSetArguments(int intDeviceId, char* strArguments){
	return false;
}

int __stdcall devAddDevice(){
	return tdAddDevice();
}

bool __stdcall devRemoveDevice(int intDeviceId){
	return tdRemoveDevice(intDeviceId);
}

int __stdcall devMethods(int id){
	return tdMethods(id, TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM | TELLSTICK_BELL);
}

bool  __stdcall devTurnOn(int intDeviceId){
	return (tdTurnOn(intDeviceId) == TELLSTICK_SUCCESS ? true : false);
}

bool  __stdcall devTurnOff(int intDeviceId){
	return (tdTurnOff(intDeviceId) == TELLSTICK_SUCCESS ? true : false);
}

bool __stdcall devBell(int intDeviceId){
	return (tdBell(intDeviceId) == TELLSTICK_SUCCESS ? true : false);
}

bool __stdcall devDim(int intDeviceId, unsigned char level){
	return (tdDim(intDeviceId, level) == TELLSTICK_SUCCESS ? true : false);
}
