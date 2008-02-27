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
