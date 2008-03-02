#include "TelldusSettings.h"
#include "../DeviceNexa.h"
#include "../DeviceWaveman.h"
#include "../DeviceSartano.h"
#include "../DeviceIkea.h"


/*
* Get the requested device, when the index of the USB dongle is known
* Note that the returned Device should be deleted when not in use anymore
*/
Device* TelldusSettings::getDevice(int intDeviceId, int intDongleIndex){

	Device* dev = 0;
	
	try{
		char* vendor = getVendor(intDeviceId);
		
		int* args = getArguments(intDeviceId);
		
		//each new brand must be added here
		if (strcmp(vendor, "Nexa") == 0){
			dev = new DeviceNexa((int)args[0], (int)args[1], intDongleIndex);
		} else if (strcmp(vendor, "Waveman") == 0) {
			dev = new DeviceWaveman((int)args[0], (int)args[1], intDongleIndex);
		} else if (strcmp(vendor, "Sartano") == 0) {
			dev = new DeviceSartano((int)args[0], (int)args[1], intDongleIndex);
		} else if (strcmp(vendor, "Ikea") == 0) {
			dev = new DeviceIkea((int)args[0], (int)args[1], (int)args[2], intDongleIndex);
		}
	}
	catch(...){
		throw;
	}
	return dev;
}

/*
* Get the name of the device
*/
char* TelldusSettings::getName(int intDeviceId){
	return getStringSetting(intDeviceId, "name");
}

/*
* Set the name of the device
*/
bool TelldusSettings::setName(int intDeviceId, char* strNewName){
	return setStringSetting(intDeviceId, "name", strNewName);
}


/*
* Get the device vendor
*/
char* TelldusSettings::getVendor(int intDeviceId){
	return getStringSetting(intDeviceId, "vendor");
}

/*
* Set the device vendor
*/
bool TelldusSettings::setVendor(int intDeviceId, char* strVendor){
	return setStringSetting(intDeviceId, "vendor", strVendor);
}

/*
* Get the device model
*/
char* TelldusSettings::getModel(int intDeviceId){
	return getStringSetting(intDeviceId, "model");
}

/*
* Set the device model
*/
bool TelldusSettings::setModel(int intDeviceId, char* strModel){
	return setStringSetting(intDeviceId, "model", strModel);
}
