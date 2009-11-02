//
// C++ Implementation: controller
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "TellStickDuo.h"
#include "Manager.h"
#include <unistd.h>
#include <stdlib.h>

#ifdef _WINDOWS
typedef HANDLE EVENT_HANDLE;
#endif

using namespace TelldusCore;

namespace TelldusCore {
	class PrivateTellStickDuoListener: public QThread {
		public:
			PrivateTellStickDuoListener( TellStickDuo *parent );
			~PrivateTellStickDuoListener();
			void stop();
		protected:
			void run();
		private:
			void processData( const std::string &data );
		
			TellStickDuo *parent;
			EVENT_HANDLE eh;
			bool running;
			QMutex mutex;
			std::string message;
	};
}

TellStickDuo::TellStickDuo(const std::string &serial)
	: Controller(),
	ftHandle(0),
	open(false),
	listener(0),
	fwVersion(0)
{
	FT_STATUS ftStatus = FT_OK;
	
	char *tempSerial = new char[serial.size()+1];
#ifdef _WINDOWS
	strcpy_s(tempSerial, serial.size()+1, serial.c_str());
#else
	strcpy(tempSerial, serial.c_str());
#endif

	ftStatus = FT_OpenEx(tempSerial, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
	delete tempSerial;
	

	if (ftStatus == FT_OK) {
		open = true;
		FT_SetBaudRate(ftHandle, 9600);
		FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0);
		
		listener = new PrivateTellStickDuoListener(this);
		listener->start();
		
		char message[] = "V+";
		DWORD bytesWritten;
		FT_Write(ftHandle, message, sizeof(message), &bytesWritten);
	} else {	
		printf("Open: %d - %d\n", (int)ftStatus, connected());
	}
}


TellStickDuo::~TellStickDuo() {
	if (listener) {
		listener->stop();
		delete listener;
	}
	if (connected()) {
		FT_Close(ftHandle);
	}
}

int TellStickDuo::firmwareVersion() {
	return fwVersion;
}

int TellStickDuo::send( const std::string &message ) {
	return TELLSTICK_SUCCESS;
}

bool TellStickDuo::connected() const {
	return open;
}

PrivateTellStickDuoListener::PrivateTellStickDuoListener( TellStickDuo *p )
	: QThread(),
	parent(p),
	running(false)
{
}

PrivateTellStickDuoListener::~PrivateTellStickDuoListener() {
	if (running) {
		stop();
	}
}

void PrivateTellStickDuoListener::stop() {
	if (running) {
		QMutexLocker locker(&mutex);
		running = false;
		//Unlock the wait-condition
		pthread_cond_broadcast(&eh.eCondVar);
	}
	wait();
}

void PrivateTellStickDuoListener::run() {
	DWORD dwBytesInQueue = 0;
	DWORD dwBytesRead = 0;
	char *buf = 0;
	
	pthread_mutex_init(&eh.eMutex, NULL);
	pthread_cond_init(&eh.eCondVar, NULL);
	
	{
		QMutexLocker locker(&mutex);
		running = true;
	}

	while(running) {
		FT_SetEventNotification(parent->ftHandle, FT_EVENT_RXCHAR, (PVOID)&eh);
		pthread_mutex_lock(&eh.eMutex);
		pthread_cond_wait(&eh.eCondVar, &eh.eMutex);
		pthread_mutex_unlock(&eh.eMutex);
		
		FT_GetQueueStatus(parent->ftHandle, &dwBytesInQueue);
		if (dwBytesInQueue <= 1) {
			continue;
		}
		
		buf = (char*)malloc(sizeof(buf) * (dwBytesInQueue+1));
		memset(buf, 0, dwBytesInQueue+1);
		FT_Read(parent->ftHandle, buf, dwBytesInQueue, &dwBytesRead);
		processData( buf );
		free(buf);
	}
}

void PrivateTellStickDuoListener::processData( const std::string &data ) {
	for (uint i = 0; i < data.length(); ++i) {
		if (data[i] == 13) { // Skip \r
			continue;
		} else if (data[i] == 10) { // \n found
			if (message.substr(0,2).compare("+V") == 0) {
				parent->fwVersion = atoi(message.substr(2).c_str());
			} else if (message.substr(0,2).compare("+R") == 0) {
				Manager *manager = Manager::getInstance();
				manager->parseMessage(message.substr(2));
			}
			message.clear();
		} else { // Append the character
			message.append( 1, data[i] );
		}
	}
}
