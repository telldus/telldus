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
#include "../client/telldus-core.h"
#include <string.h>

#include "ftd2xx.h"

class TellStick::PrivateData {
public:
	bool open;
	FT_HANDLE ftHandle;
};

TellStick::TellStick( const TellStickDescriptor &td ) {
	d = new PrivateData;
	d->open = false;

	char *tempSerial = new char[td.serial.size()+1];
#ifdef _WINDOWS
	strcpy_s(tempSerial, td.serial.size()+1, td.serial.c_str());
#else
	strcpy(tempSerial, td.serial.c_str());
	FT_SetVIDPID(td.vid, td.pid);
#endif
	FT_STATUS ftStatus = FT_OpenEx(tempSerial, FT_OPEN_BY_SERIAL_NUMBER, &d->ftHandle);
	delete tempSerial;
	if (ftStatus == FT_OK) {
		d->open = true;
		FT_SetFlowControl(d->ftHandle, FT_FLOW_NONE, 0, 0);
		FT_SetTimeouts(d->ftHandle,5000,0);
	}
	
	if (d->open) {
 		setBaud(4800);
	}
}

TellStick::~TellStick() {
	if (d->open) {
	}
	delete d;
}

void TellStick::setBaud( int baud ) {
	FT_SetBaudRate(d->ftHandle, baud);
}

int TellStick::firmwareVersion() {
	return 1;
}

bool TellStick::isOpen() const {
	return d->open;
}

int TellStick::send( const std::string &strMessage ) {
	if (!d->open) {
		return TELLSTICK_ERROR_NOT_FOUND;
	}
	bool c = true;

	char *tempMessage = (char *)malloc(sizeof(char) * (strMessage.size()+1));
#ifdef _WINDOWS
	strcpy_s(tempMessage, strMessage.size()+1, strMessage.c_str());
#else
	strcpy(tempMessage, strMessage.c_str());
#endif

	ULONG bytesWritten, bytesRead;
	char in;
	FT_STATUS ftStatus;
	ftStatus = FT_Write(d->ftHandle, tempMessage, (DWORD)strMessage.length(), &bytesWritten);
	free(tempMessage);

	while(c) {
		ftStatus = FT_Read(d->ftHandle,&in,1,&bytesRead);
		if (ftStatus == FT_OK) {
			if (bytesRead == 1) {
				if (in == '\n') {
					break;
				}
			} else { //Timeout
				c = false;
			}
		} else { //Error
			c = false;
		}
	}

	if (!c) {
		return TELLSTICK_ERROR_COMMUNICATION;
	}
	return TELLSTICK_SUCCESS;
}

std::list<TellStickDescriptor> TellStick::findAll() {
	std::list<TellStickDescriptor> tellstick = findAllByVIDPID(0x1781, 0x0C30);

	std::list<TellStickDescriptor> duo = findAllByVIDPID(0x1781, 0x0C31);
	for(std::list<TellStickDescriptor>::const_iterator it = duo.begin(); it != duo.end(); ++it) {
		tellstick.push_back(*it);
	}

	return tellstick;
	
}

std::list<TellStickDescriptor> TellStick::findAllByVIDPID( int vid, int pid ) {
	std::list<TellStickDescriptor> retval;
	
	FT_HANDLE fthHandle = 0;
	FT_STATUS ftStatus = FT_OK;
	DWORD dwNumberOfDevices = 0;

#ifndef _WINDOWS
	FT_SetVIDPID(vid, pid);
#endif

	ftStatus = FT_CreateDeviceInfoList(&dwNumberOfDevices);
	if (ftStatus != FT_OK) {
		return retval;
	}
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
		ftStatus = FT_Close(fthHandle);
		if (ftStatus != FT_OK) {
			continue;
		}
		if (pData.VendorId == vid && pData.ProductId == pid) {
			TellStickDescriptor td;
			td.vid = vid;
			td.pid = pid;
			td.serial = pData.SerialNumber;
			retval.push_back(td);
		}
	}
	return retval;
}
