#include "../Device.h"
#include <string>
#include <sstream> 
#include <vector>
#include <iostream>
#include <fstream>

#ifdef _WINDOWS
#include "..\StdAfx.h"
#endif
#include "ftd2xx.h"

int getDongleIndex();

/*
* Send message to the USB dongle
*/
int Device::send(char* strMessage){

	try{
		FT_STATUS ftStatus = FT_OK;
		FT_HANDLE fthHandle = 0;
		
		int intDongleIndex = getDongleIndex();
		if (intDongleIndex < 0) {
			return TELLSTICK_ERROR_NOT_FOUND;
		}

		ftStatus = FT_Open(intDongleIndex, &fthHandle);
		int intBaudRate = 4800;	//always 4800
		ftStatus = FT_SetBaudRate(fthHandle, intBaudRate);
		ULONG bytesWritten;

		int intLen = (int)strlen(strMessage);
		ftStatus = FT_Write(fthHandle, strMessage, intLen, &bytesWritten);
  
		ftStatus = FT_Close(fthHandle);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_SUCCESS;
}

/*
* Get the device index of the Telldus dongle (the first one if many are connected)
* -1 if no such device exists
*/
int getDongleIndex(){
	int intReturn = -1;
	FT_HANDLE fthHandle = 0;
	FT_STATUS ftStatus = FT_OK;

	try{
		DWORD dwNumberOfDevices = 0;

#ifndef _WINDOWS
		FT_SetVIDPID(0x1781, 0x0C30);
#endif
		ftStatus = FT_CreateDeviceInfoList(&dwNumberOfDevices);
		if (ftStatus == FT_OK) { 
			for (int i = 0; i < (int)dwNumberOfDevices; i++) {  
				
				FT_PROGRAM_DATA pData;
				char ManufacturerBuf[32]; 
				char ManufacturerIdBuf[16]; 
				char DescriptionBuf[64]; 
				char SerialNumberBuf[16]; 
				
				pData.Signature1 = 0x00000000; 
				pData.Signature2 = 0xffffffff; 
				pData.Version = 0x00000002;      // EEPROM structure with FT232R extensions 
				pData.Manufacturer = ManufacturerBuf; 
				pData.ManufacturerId = ManufacturerIdBuf; 
				pData.Description = DescriptionBuf; 
				pData.SerialNumber = SerialNumberBuf; 
				
				ftStatus = FT_Open(i, &fthHandle);
				ftStatus = FT_EE_Read(fthHandle, &pData);
				if(ftStatus == FT_OK){
					if(pData.VendorId == 6017 && pData.ProductId == 3120){
						intReturn = i;
						ftStatus = FT_Close(fthHandle);
						break;
					}
				}
				ftStatus = FT_Close(fthHandle);
			}
		}
	}
	catch(...){
		throw;
	}
	return intReturn;
}

