#include "../Device.h"
#include <iostream>
#include <fcntl.h>
#include <termios.h>

/*
* Send message to the USB dongle
*/
void Device::send(char* strMessage) {
	int fd = -1;
	struct termios tio;
	
	debugLog(strMessage);
	
	if( 0 > ( fd = open( "/dev/tellstick", O_RDWR ) ) ) {
		return;
	}
	
	/* adjust serial port parameters */
	bzero(&tio, sizeof(tio)); /* clear struct for new port settings */
	tio.c_cflag = B4800 | CS8 | CLOCAL | CREAD; /* CREAD not used yet */
	tio.c_iflag = IGNPAR;
	tio.c_oflag = 0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&tio);

	write(fd, strMessage, strlen(strMessage));
	
	close(fd);
}

/*
* Get the device index of the Telldus dongle (the first one if many are connected)
* -1 if no such device exists
*/
int Device::getDongleIndex(){
	int intReturn = -1;
/*	FT_HANDLE fthHandle = 0;
	FT_STATUS ftStatus = FT_OK;

	try{
		DWORD dwNumberOfDevices = 0;
		
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
	}*/
	return intReturn;
}

//only for debugging purpose
void Device::debugLog(char* debugstring){
	std::cout << debugstring << std::endl;
}

