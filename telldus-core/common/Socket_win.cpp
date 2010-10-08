#include "Socket.h"
#include "Event.h"

#include <windows.h>
#include <AccCtrl.h>
#include <Aclapi.h>

#define BUFSIZE 512

//TODO: using namespace TelldusCore;

class Socket::PrivateData {
public:
	HANDLE hPipe;
};

Socket::Socket(SOCKET_T hPipe)
{
	d = new PrivateData;
	d->hPipe = hPipe;
	
}

Socket::~Socket(void){
	delete d;
}

std::wstring Socket::read() {
	wchar_t buf[BUFSIZE];
	int result;
	DWORD cbBytesRead = 0;
	OVERLAPPED oOverlap; 
	HANDLE readEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	oOverlap.hEvent = readEvent;
	bool fSuccess = false;

	memset(&buf, 0, BUFSIZE);

	ReadFile( d->hPipe, &buf, sizeof(wchar_t)*BUFSIZE, &cbBytesRead, &oOverlap);

	result = WaitForSingleObject(oOverlap.hEvent, 10000);
	if (result == WAIT_TIMEOUT) {
		CloseHandle(readEvent);
		return L"";
	}
	fSuccess = GetOverlappedResult(d->hPipe, &oOverlap, &cbBytesRead, false);
	if (!fSuccess) {
		DWORD err = GetLastError();
		if (err == ERROR_BROKEN_PIPE) {
			//TODO: Connection closed
		}
		buf[0] = 0;
	}
	CloseHandle(readEvent);
	return buf;
}