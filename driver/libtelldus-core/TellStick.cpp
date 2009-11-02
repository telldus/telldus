//
// C++ Implementation: TellStick
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "TellStick.h"
#include "ftd2xx.h"
#include <string>


using namespace TelldusCore;

TellStick::TellStick() {
}


TellStick::~TellStick() {
}


int TellStick::findFirstDevice() {
	//TellStick
	std::string serial = findByVIDPID(0x1781, 0x0C30);
	if (serial.length() > 0) {
		return 1;
	}
	//TellStick Duo
	serial = findByVIDPID(0x1781, 0x0C31);
	if (serial.length() > 0) {
		return 2;
	}
	return 0;
}

std::string TellStick::findByVIDPID( int vid, int pid ) {
	std::string retval = "";
	
#ifdef _LINUX
	ftdi_context ftdic;
	ftdi_init(&ftdic);
	
	int ret = ftdi_usb_open(&ftdic, vid, pid);
	if (ret == 0) {
		retval = "1";
	
		ftdi_usb_close(&ftdic);
	}
	
	ftdi_deinit(&ftdic);
	
#else
	FT_HANDLE fthHandle = 0;
	FT_STATUS ftStatus = FT_OK;
	
	try{
		DWORD dwNumberOfDevices = 0;
		
#ifndef _WINDOWS
		FT_SetVIDPID(vid, pid);
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
					if(pData.VendorId == vid && pData.ProductId == pid){
						ftStatus = FT_Close(fthHandle);
						retval = pData.SerialNumber;
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
#endif
	return retval;
}
