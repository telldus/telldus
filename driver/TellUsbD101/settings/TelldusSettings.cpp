#include "TelldusSettings.h"
#include "../DeviceNexa.h"
#include "../DeviceWaveman.h"
#include "../DeviceSartano.h"
#include "../DeviceIkea.h"
#include <string.h>
#include <stdlib.h>

/*
* Get the requested device
* Note that the returned Device should be deleted when not in use anymore
*/
Device* TelldusSettings::getDevice(int intDeviceId){

	Device* dev = NULL;

	try{
		char* vendor = getVendor(intDeviceId);

		//each new brand must be added here
		if (strcmp(vendor, "Nexa") == 0){
			char *strHouse = getArgument(intDeviceId, "nexa_house");
			char *strCode = getArgument(intDeviceId, "nexa_unit");
			dev = new DeviceNexa(strHouse, strCode);
			free(strHouse);
			free(strCode);
		
		} else if (strcmp(vendor, "Waveman") == 0) {
			char *strHouse = getArgument(intDeviceId, "nexa_house");
			char *strCode = getArgument(intDeviceId, "nexa_unit");
			dev = new DeviceWaveman(strHouse, strCode);
			free(strHouse);
			free(strCode);

		} else if (strcmp(vendor, "Sartano") == 0) {
			char *strCode = getArgument(intDeviceId, "sartano_code");
			dev = new DeviceSartano(strCode);
			free(strCode);

		} else if (strcmp(vendor, "Ikea") == 0) {
			char *strSystem = getArgument(intDeviceId, "ikea_system");
			char *strUnits = getArgument(intDeviceId, "ikea_units");
			char *strFade = getArgument(intDeviceId, "ikea_fade");
			dev = new DeviceIkea(strSystem, strUnits, strFade);
			free(strSystem);
			free(strUnits);
			free(strFade);

		} else {
			free(vendor);
			return NULL;
		}
		free(vendor);

#ifdef _LINUX
		dev->setDevice( getSetting("deviceNode") );
#endif

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
	return getStringSetting(intDeviceId, "name", false);
}

/*
* Set the name of the device
*/
bool TelldusSettings::setName(int intDeviceId, const char* strNewName){
	return setStringSetting(intDeviceId, "name", strNewName, false);
}

/*
* Get the device vendor
*/
char* TelldusSettings::getVendor(int intDeviceId){
	return getStringSetting(intDeviceId, "vendor", false);
}

/*
* Set the device vendor
*/
bool TelldusSettings::setVendor(int intDeviceId, const char* strVendor){
	return setStringSetting(intDeviceId, "vendor", strVendor, false);
}

/*
* Get the device model
*/
char* TelldusSettings::getModel(int intDeviceId){
	return getStringSetting(intDeviceId, "model", false);
}

/*
* Set the device model
*/
bool TelldusSettings::setModel(int intDeviceId, const char* strModel){
	return setStringSetting(intDeviceId, "model", strModel, false);
}

/*
* Set device argument
*/
bool TelldusSettings::setArgument(int intDeviceId, const char *strName, const char *strValue){
	return setStringSetting(intDeviceId, strName, strValue, true);
}

/*
* Get device argument
*/
char* TelldusSettings::getArgument(int intDeviceId, const char *strName) {
	return getStringSetting(intDeviceId, strName, true);
}
