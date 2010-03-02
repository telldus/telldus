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

class TelldusCore::TellStickDescriptor {
public:
	bool found;
	std::string serial;
	int vid, pid;
};

class TelldusCore::PrivateVars {
public:
	bool open;
	TellStickHandle ftHandle;
	TellStickDescriptor descriptor;
};

TellStick::TellStick( const TellStickDescriptor &td ) {
	d = new PrivateVars;
	d->open = false;
	d->descriptor = td;

#ifdef LIBFTDI
	ftdi_init(&d->ftHandle);
	
	int ret = ftdi_usb_open_desc(&d->ftHandle, td.vid, td.pid, NULL, td.serial.c_str());
	if (ret < 0) {
		ftdi_deinit(&d->ftHandle);
		return;
	}
	d->open = true;
	ftdi_usb_reset( &d->ftHandle );
	ftdi_disable_bitbang( &d->ftHandle );
#else
		
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
#endif
	
	if (d->open) {
		setBaud(4800);
	}
}

TellStick::~TellStick() {
	if (d->open) {
#ifdef LIBFTDI
		ftdi_usb_close(&d->ftHandle);
		ftdi_deinit(&d->ftHandle);
#else
		FT_Close(d->ftHandle);
#endif
	}
	delete d;
}

int TellStick::vid() const {
	return d->descriptor.vid;
}

int TellStick::pid() const {
	return d->descriptor.pid;
}

std::string TellStick::serial() const {
	return d->descriptor.serial;
}

bool TellStick::open() const {
	return d->open;
}

TellStick *TellStick::findFirstDevice(int vid, int pid) {
	TellStick *tellstick = 0;

	if (vid == 0 && pid == 0) {
		tellstick = findFirstDevice(0x1781, 0x0C30);
		if (tellstick) {
			return tellstick;
		}
#ifdef TELLSTICK_DUO
		tellstick = findFirstDevice(0x1781, 0x0C31);
#endif
		return tellstick;

	}

	TellStickDescriptor d = findByVIDPID(vid, pid);
	if (d.found) {
#ifdef TELLSTICK_DUO
		if (pid == 0x0C31) {
			return new TellStickDuo(d);
		}
#endif
		return new TellStick(d);
	}
	return 0;
}

TellStick *TellStick::loadBy(int vid, int pid, const std::string &serial) {
	if (vid != 0x1781) {
		return 0;
	}

	TellStick *tellstick = 0;

	if (serial.length() == 0) {
		tellstick = TellStick::findFirstDevice();
	} else {
		TellStickDescriptor d;
		d.vid = vid;
		d.pid = pid;
		d.serial = serial;

		if (pid == 0x0C30) {
			tellstick = new TellStick(d);
#ifdef TELLSTICK_DUO
		} else if (pid == 0x0C31) {
			tellstick = new TellStickDuo(d);
#endif
		}
	}
	if (!tellstick) {
		return 0;
	}
	if (tellstick->open()) {
		return tellstick;
	}
	delete tellstick;
	return 0;
}

TellStickDescriptor TellStick::findByVIDPID( int vid, int pid ) {
	TellStickDescriptor retval;
	retval.found = false;
	retval.vid = vid;
	retval.pid = pid;
	
#ifdef LIBFTDI
	ftdi_context ftdic;
	struct ftdi_device_list *devlist, *curdev;
	char serialBuffer[10];
	ftdi_init(&ftdic);
	
	int ret = ftdi_usb_find_all(&ftdic, &devlist, vid, pid);
	if (ret > 0) {
		for (curdev = devlist; curdev != NULL; curdev = curdev->next) {
			ret = ftdi_usb_get_strings(&ftdic, curdev->dev, NULL, 0, NULL, 0, serialBuffer, 10);
			if (ret != 0) {
				continue;
			}
			retval.serial = serialBuffer;
			break;
		}
		retval.found = true;
	}
	
	ftdi_list_free(&devlist);
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
	return 1;
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
	ret = ftdi_write_data( &d->ftHandle, tempMessage, strMessage.length() ) ;
	if(ret < 0) {
		c = false;
	} else if(ret != strMessage.length()) {
		fprintf(stderr, "wierd send length? retval %i instead of %d\n",
				ret, (int)strMessage.length());
	}

	delete[] tempMessage;

	int retrycnt = 500;
	unsigned char in;
	while(c && --retrycnt) {
		ret = ftdi_read_data( &d->ftHandle, &in, 1);
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
	if (!retrycnt) {
		c = false;
	}
#else
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
#endif
	
	if (!c) {
		return TELLSTICK_ERROR_COMMUNICATION;
	}
	return TELLSTICK_SUCCESS;
}

void TelldusCore::TellStick::setBaud(int baud) {
#ifdef LIBFTDI
	int ret = ftdi_set_baudrate(&d->ftHandle, baud);
	if(ret != 0) {
		fprintf(stderr, "set Baud failed, retval %i\n", ret);
	}
#else
	FT_SetBaudRate(d->ftHandle, baud);
#endif
}

TellStickHandle TelldusCore::TellStick::handle() const {
	return d->ftHandle;
}
