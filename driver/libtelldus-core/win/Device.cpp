#include "../Device.h"
#include <string>
#include <sstream> 
#include <vector>
#include <iostream>
#include <fstream>

#ifdef _WINDOWS
#include "..\StdAfx.h"
#else
#include "WinTypes.h"
#endif
#include "ftd2xx.h"

int getDongleIndex();

using namespace TelldusCore;

/*
* Send message to the USB dongle
*/
int Device::send(const std::string &strMessage){

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

		char *tempMessage = (char *)malloc(sizeof(char) * (strMessage.size()+1));
		strcpy(tempMessage, strMessage.c_str());
		ftStatus = FT_Write(fthHandle, tempMessage, (DWORD)strMessage.length(), &bytesWritten);
		free(tempMessage);
  
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

