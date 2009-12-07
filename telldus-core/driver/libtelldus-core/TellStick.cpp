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
#include "TellStickDuo.h"
#include "telldus-core.h"
#include <string>
#include <stdlib.h>

using namespace TelldusCore;

class TelldusCore::PrivateVars {
	public:
		bool open;
#ifdef LIBFTDI
		ftdi_context ftdic;
#else
		FT_HANDLE ftHandle;
#endif
};

class TelldusCore::TellStickDescriptor {
public:
	bool found;
#ifdef LIBFTDI
	int vid, pid;
#else
	std::string serial;
#endif
	
};

TellStick::TellStick( const TellStickDescriptor &td ) {
	d = new PrivateVars;
	d->open = false;
#ifdef LIBFTDI
	ftdi_init(&d->ftdic);
	
	int ret = ftdi_usb_open_desc(&d->ftdic, td.vid, td.pid, NULL, NULL);
	if (ret < 0) {
		ftdi_deinit(&d->ftdic);
		return;
	}
	d->open = true;
	ftdi_usb_reset( &d->ftdic );
	ftdi_disable_bitbang( &d->ftdic );
#else
		
	char *tempSerial = new char[td.serial.size()+1];
#ifdef _WINDOWS
	strcpy_s(tempSerial, td.serial.size()+1, td.serial.c_str());
#else
	strcpy(tempSerial, td.serial.c_str());
#endif
	FT_STATUS ftStatus = FT_OpenEx(tempSerial, FT_OPEN_BY_SERIAL_NUMBER, &d->ftHandle);
	delete tempSerial;
	if (ftStatus == FT_OK) {
		d->open = true;
		FT_SetFlowControl(d->ftHandle, FT_FLOW_NONE, 0, 0);
		FT_SetTimeouts(d->ftHandle,5000,0);
	}
#endif
	
	if (d->open) {
		setBaud(4800);
	}
}

TellStick::~TellStick() {
	if (d->open) {
#ifdef LIBFTDI
		ftdi_usb_close(&d->ftdic);
		ftdi_deinit(&d->ftdic);
#else
		FT_Close(d->ftHandle);
#endif
	}
	delete d;
}

TellStick *TellStick::findFirstDevice() {
	//TellStick
	TellStickDescriptor d = findByVIDPID(0x1781, 0x0C30);
	if (d.found) {
		return new TellStick(d);
	}
#ifdef TELLSTICK_DUO
	//TellStick Duo
	d = findByVIDPID(0x1781, 0x0C31);
	if (d.found) {
		return new TellStickDuo(d);
	}
#endif
	return 0;
}

TellStickDescriptor TellStick::findByVIDPID( int vid, int pid ) {
	TellStickDescriptor retval;
	retval.found = false;
	
#ifdef LIBFTDI
	ftdi_context ftdic;
	ftdi_init(&ftdic);
	
	int ret = ftdi_usb_open(&ftdic, vid, pid);
	if (ret == 0) {
		retval.found = true;
		retval.vid = vid;
		retval.pid = pid;
	
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
						retval.found = true;
						retval.serial = pData.SerialNumber;
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

int TelldusCore::TellStick::firmwareVersion() {
	1;
}

int TelldusCore::TellStick::send(const std::string & strMessage) {
	if (!d->open) {
		return TELLSTICK_ERROR_NOT_FOUND;
	}
	
	
	bool c = true;
#ifdef LIBFTDI
	unsigned char *tempMessage = new unsigned char[strMessage.size()];
 	memcpy(tempMessage, strMessage.c_str(), strMessage.size());

	int ret;
	ret = ftdi_write_data( &d->ftdic, tempMessage, strMessage.length() ) ;
	if(ret < 0) {
		c = false;
	} else if(ret != strMessage.length()) {
		fprintf(stderr, "wierd send length? retval %i instead of %d\n",
				ret, strMessage.length());
	}

	delete[] tempMessage;

	int retrycnt = 5000;
	unsigned char in;
	while(c && retrycnt--) {
		ret = ftdi_read_data( &d->ftdic, &in, 1);
		if (ret > 0) {
			if (in == '\n') {
				break;
			}
		} else if(ret == 0) { // No data available
			usleep(100);
		} else { //Error
			c = false;
		}
	}
#else
	char *tempMessage = (char *)malloc(sizeof(char) * (strMessage.size()+1));
	strcpy(tempMessage, strMessage.c_str());
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
#endif
	
	if (!c) {
		return TELLSTICK_ERROR_COMMUNICATION;
	}
	return TELLSTICK_SUCCESS;
}

void TelldusCore::TellStick::setBaud(int baud) {
#ifdef LIBFTDI
	int ret = ftdi_set_baudrate(&d->ftdic, baud);
	if(ret != 0) {
		fprintf(stderr, "set Baud failed, retval %i\n", ret);
	}
#else
	FT_SetBaudRate(d->ftHandle, baud);
#endif
}

FT_HANDLE TelldusCore::TellStick::handle() const {
	return d->ftHandle;
}
