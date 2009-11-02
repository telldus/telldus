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
#include "telldus-core.h"
#include <string>

using namespace TelldusCore;

class TelldusCore::PrivateVars {
	public:
		bool open;
#ifdef _LINUX
		ftdi_context ftdic;
#endif
};

TellStick::TellStick( int vid, int pid ) {
	d = new PrivateVars;
	d->open = false;
#ifdef _LINUX
	ftdi_init(&d->ftdic);
	
	int ret = ftdi_usb_open_desc(&d->ftdic, vid, pid, NULL, NULL);
	if (ret < 0) {
		ftdi_deinit(&d->ftdic);
		return;
	}
	d->open = true;
	ftdi_usb_reset( &d->ftdic );
	printf("DisBB %i\n", ftdi_disable_bitbang( &d->ftdic ));
 	printf("Baud %i\n", ftdi_set_baudrate(&d->ftdic, 4800));

#endif
}


TellStick::~TellStick() {
	if (d->open) {
#ifdef _LINUX
		ftdi_usb_close(&d->ftdic);
		ftdi_deinit(&d->ftdic);
		printf("Cloooooooooooosing\n");
#endif
	}
	delete d;
}


TellStick *TellStick::findFirstDevice() {
	//TellStick
	std::string serial = findByVIDPID(0x1781, 0x0C30);
	if (serial.length() > 0) {
		return new TellStick(0x1781, 0x0C30);
	}
	//TellStick Duo
	serial = findByVIDPID(0x1781, 0x0C31);
	if (serial.length() > 0) {
		return 0;
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

int TelldusCore::TellStick::firmwareVersion() {
	1;
}

int TelldusCore::TellStick::send(const std::string & message) {
	if (!d->open) {
		return TELLSTICK_ERROR_NOT_FOUND;
	}
	
	unsigned char *tmp = new unsigned char[message.size()];
 	memcpy(tmp, message.c_str(), message.size());
	unsigned char t[] = {'S','$','k','$','k','$','k','$','k','$','k','$','k','$','k','$','k','$','k','$','k','$','k','$','k','k','$','k','$','k','$','k','$','k','$','k','$','k','$','k','$','$','k','k','$','$','k','k','$','$','k','+',0};
	
	int ret = ftdi_write_data( &d->ftdic, t, 52 );	
	sleep(3);
// 	ftdi_usb_purge_tx_buffer(&d->ftdic);
// 	ftdi_usb_purge_rx_buffer(&d->ftdic);
	printf(" %i (%i), %s\n", ret, message.size(), t);

	delete[] tmp;
	return TELLSTICK_SUCCESS;
}
