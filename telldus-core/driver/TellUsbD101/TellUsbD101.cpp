//DLL entry point

#include "TellUsbD101.h"
#include "../libtelldus-core/telldus-core.h"

int __stdcall devGetNumberOfDevices(void){
	int intReturn = -1;
	return intReturn;
}

int __stdcall devGetDeviceId(int intDeviceIndex){
	int intReturn = -1;
	return intReturn;
}

char * __stdcall devGetName(int intDeviceId){
	char* strReturn;
	return strReturn;
}

char* __stdcall devGetVendor(int intDeviceId){
	char* strReturn = "";
	return strReturn;
}

char* __stdcall devGetModel(int intDeviceId){
	char* strReturn = "";
	return strReturn;
}

int __stdcall devGetNumberOfArguments(int intDeviceId){
	int intReturn = 0;
	return intReturn;
}

int __stdcall devGetArgument(int intDeviceId, int intArgumentIndex){
	int intReturn;
	return intReturn;
}

bool __stdcall devSetName(int intDeviceId, char* strNewName){
	bool blnSuccess = false;
	return blnSuccess;
}

bool __stdcall devSetVendor(int intDeviceId, char* strVendor){
	bool blnSuccess = false;
	return blnSuccess;
}

bool __stdcall devSetModel(int intDeviceId, char* strNewModel){
	bool blnSuccess = false;
	return blnSuccess;
}

bool __stdcall devSetArguments(int intDeviceId, char* strArguments){
	return false;
}

int __stdcall devAddDevice(){
	int intNewDeviceId = -1;
	return intNewDeviceId;
}

bool __stdcall devRemoveDevice(int intDeviceId){
	bool blnSuccess = false;
	return blnSuccess;
}

int __stdcall devMethods(int id){
	int intMethods = 0;
	return intMethods;
}

bool  __stdcall devTurnOn(int intDeviceId){
	return false;
}

bool  __stdcall devTurnOff(int intDeviceId){
	return false;
}

bool __stdcall devBell(int intDeviceId){
	return false;
}

bool __stdcall devDim(int intDeviceId, unsigned char level){
	return false;
}
