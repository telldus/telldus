#include "Socket.h"

#include <windows.h>
#include <AccCtrl.h>
#include <Aclapi.h>

#define BUFSIZE 512

using namespace TelldusCore;

class Socket::PrivateData {
public:
	HANDLE hPipe;
	bool connected;
};

Socket::Socket() {
	d = new PrivateData;
	d->connected = false;
}

Socket::Socket(SOCKET_T hPipe)
{
	d = new PrivateData;
	d->hPipe = hPipe;
	
}


Socket::~Socket(void){
	if (d->hPipe != INVALID_HANDLE_VALUE) {
		CloseHandle(d->hPipe);
	}
	delete d;
}

void Socket::connect(const std::wstring &server){
	bool  fSuccess = false;

	std::wstring name(L"\\\\.\\pipe\\" + server);
	d->hPipe = CreateFile(
		(const wchar_t *)name.c_str(),           // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE, 
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		FILE_FLAG_OVERLAPPED, // default attributes 
		NULL);          // no template file 

	if (d->hPipe == INVALID_HANDLE_VALUE) {
		return;
	}
	
	DWORD dwMode = PIPE_READMODE_MESSAGE; 
	fSuccess = SetNamedPipeHandleState( 
      d->hPipe, // pipe handle 
      &dwMode,  // new pipe mode 
      NULL,     // don't set maximum bytes 
      NULL);    // don't set maximum time 

	if (!fSuccess) {
		return;
	}
	d->connected = true;
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

void Socket::write(const std::wstring &msg){
	
	OVERLAPPED oOverlap; 
	DWORD bytesWritten = 0;
	int result;
	bool fSuccess;

	HANDLE writeEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	oOverlap.hEvent = writeEvent;
	
	WriteFile(d->hPipe, msg.data(), (DWORD)msg.length()*sizeof(wchar_t), &bytesWritten, &oOverlap);

	result = WaitForSingleObject(writeEvent, 10000);
	if (result == WAIT_TIMEOUT) {
		CloseHandle(writeEvent);
		//d->connected = false;
		return;
	}
	fSuccess = GetOverlappedResult(d->hPipe, &oOverlap, &bytesWritten, false);
	CloseHandle(writeEvent);
	if (!fSuccess) {
		//d->connected = false;
		return;	
	}
}