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
#include "Mutex.h"
#include "Strings.h"
#include "../client/telldus-core.h"
#include <string.h>
#include <stdlib.h>

#include "ftd2xx.h"

class TellStick::PrivateData {
public:
	bool open, running;
	int vid, pid, fwVersion;
	std::string serial, message;
	FT_HANDLE ftHandle;
	TelldusCore::Mutex mutex;

#ifdef _WINDOWS
	HANDLE eh;
#else
//#include <unistd.h>
	struct {
		pthread_cond_t eCondVar;
		pthread_mutex_t eMutex;
	} eh;
#endif
};

TellStick::TellStick(int controllerId, Event *event, const TellStickDescriptor &td )
	:Controller(controllerId, event)
{
	d = new PrivateData;
#ifdef _WINDOWS
	d->eh = CreateEvent( NULL, false, false, NULL );
#else
	pthread_mutex_init(&d->eh.eMutex, NULL);
	pthread_cond_init(&d->eh.eCondVar, NULL);
#endif
	d->open = false;
	d->running = false;
	d->vid = td.vid;
	d->pid = td.pid;
	d->fwVersion = 0;
	d->serial = td.serial;

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
		if (td.pid == 0x0C31) {
			setBaud(9600);
		} else {
			setBaud(4800);
		}
		this->start();
	}
}

TellStick::~TellStick() {
	if (d->running) {
		TelldusCore::MutexLocker locker(&d->mutex);
		d->running = false;
#ifdef _WINDOWS
		SetEvent(d->eh);
#else
		pthread_cond_broadcast(&d->eh.eCondVar);
#endif
	}
	this->wait();
	if (d->open) {
		FT_Close(d->ftHandle);
	}
	delete d;
}

void TellStick::setBaud( int baud ) {
	FT_SetBaudRate(d->ftHandle, baud);
}

int TellStick::firmwareVersion() {
	return d->fwVersion;
}

int TellStick::pid() const {
	return d->pid;
}

bool TellStick::isOpen() const {
	return d->open;
}

bool TellStick::isSameAsDescriptor(const TellStickDescriptor &td) const {
	if (td.vid != d->vid) {
		return false;
	}
	if (td.pid != d->pid) {
		return false;
	}
	if (td.serial != d->serial) {
		return false;
	}
	return true;
}

void TellStick::processData( const std::string &data ) {
	for (unsigned int i = 0; i < data.length(); ++i) {
		if (data[i] == 13) { // Skip \r
			continue;
		} else if (data[i] == 10) { // \n found
			if (d->message.substr(0,2).compare("+V") == 0) {
				d->fwVersion = TelldusCore::charToInteger(d->message.substr(2).c_str());
			} else if (d->message.substr(0,2).compare("+R") == 0) {
				this->publishData(d->message.substr(2));
			} else if(d->message.substr(0,2).compare("+W") == 0) {
				this->decodePublishData(d->message.substr(2));
			}
			d->message.clear();
		} else { // Append the character
			d->message.append( 1, data[i] );
		}
	}
}

void TellStick::run() {
	d->running = true;
	DWORD dwBytesInQueue = 0;
	DWORD dwBytesRead = 0;
	char *buf = 0;

	//Send a firmware version request
	char msg[] = "V+";
	FT_Write(d->ftHandle, msg, (DWORD)strlen(msg), &dwBytesRead);

	while(1) {
#ifdef _WINDOWS
		FT_SetEventNotification(d->ftHandle, FT_EVENT_RXCHAR, d->eh);
		WaitForSingleObject(d->eh,INFINITE);
#else
		FT_SetEventNotification(d->ftHandle, FT_EVENT_RXCHAR, (PVOID)&d->eh);
		pthread_mutex_lock(&d->eh.eMutex);
		pthread_cond_wait(&d->eh.eCondVar, &d->eh.eMutex);
		pthread_mutex_unlock(&d->eh.eMutex);
#endif

		TelldusCore::MutexLocker locker(&d->mutex);
		if (!d->running) {
			break;
		}
		FT_GetQueueStatus(d->ftHandle, &dwBytesInQueue);
		if (dwBytesInQueue < 1) {
			d->mutex.unlock();
			continue;
		}
		buf = (char*)malloc(sizeof(buf) * (dwBytesInQueue+1));
		memset(buf, 0, dwBytesInQueue+1);
		FT_Read(d->ftHandle, buf, dwBytesInQueue, &dwBytesRead);
		processData( buf );
		free(buf);
	}
}

int TellStick::send( const std::string &strMessage ) {
	if (!d->open) {
		return TELLSTICK_ERROR_NOT_FOUND;
	}
	bool c = true;

	//This lock does two things
	// 1 Prevents two calls from different threads to this function
	// 2 Prevents our running thread from receiving the data we are interested in here
	TelldusCore::MutexLocker locker(&d->mutex);

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

bool TellStick::stillConnected() const {
	FT_STATUS ftStatus;
	DWORD numDevs;
	// create the device information list
	ftStatus = FT_CreateDeviceInfoList(&numDevs);
	if (ftStatus != FT_OK) {
		return false;
	}
	if (numDevs <= 0) {
		return false;
	}
	for (int i = 0; i < (int)numDevs; i++) {
		FT_HANDLE ftHandleTemp;
		DWORD flags;
		DWORD id;
		DWORD type;
		DWORD locId;
		char serialNumber[16];
		char description[64];
		// get information for device i
		ftStatus = FT_GetDeviceInfoDetail(i, &flags, &type, &id, &locId, serialNumber, description, &ftHandleTemp);
		if (ftStatus != FT_OK) {
			continue;
		}
		if (d->serial.compare(serialNumber) == 0) {
			return true;
		}
	}
	return false;
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

	FT_STATUS ftStatus = FT_OK;
	DWORD dwNumberOfDevices = 0;

#ifndef _WINDOWS
	FT_SetVIDPID(vid, pid);
#endif

	ftStatus = FT_CreateDeviceInfoList(&dwNumberOfDevices);
	if (ftStatus != FT_OK) {
		return retval;
	}
	if (dwNumberOfDevices > 0) {
		FT_DEVICE_LIST_INFO_NODE *devInfo;
		// allocate storage for list based on dwNumberOfDevices
		devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*dwNumberOfDevices); // get the device information list 
		ftStatus = FT_GetDeviceInfoList(devInfo,&dwNumberOfDevices);
		if (ftStatus == FT_OK) {
			unsigned int id = (vid << 16) | pid;
			for (unsigned int i = 0; i < dwNumberOfDevices; i++) {
				if (devInfo[i].ID != id) {
					continue;
				}
				TellStickDescriptor td;
				td.vid = vid;
				td.pid = pid;
				td.serial = devInfo[i].SerialNumber;
				retval.push_back(td);
			}
		}
		free(devInfo);
	}
	return retval;
}
