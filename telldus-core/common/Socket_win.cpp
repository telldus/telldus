#include "Socket.h"

#include <windows.h>
#include <AccCtrl.h>
#include <Aclapi.h>

#define BUFSIZE 512

using namespace TelldusCore;

class Socket::PrivateData {
public:
	HANDLE hPipe;
	HANDLE readEvent;
	bool connected;
	bool running;
};

Socket::Socket() {
	d = new PrivateData;
	d->hPipe = INVALID_HANDLE_VALUE;
	d->connected = false;
	d->running = true;
}

Socket::Socket(SOCKET_T hPipe)
{
	d = new PrivateData;
	d->hPipe = hPipe;
	d->connected = true;
	d->running = true;
}


Socket::~Socket(void){
	d->running = false;
	SetEvent(d->readEvent);	//signal for break
	if (d->hPipe != INVALID_HANDLE_VALUE) {
		CloseHandle(d->hPipe);
	}
	delete d;
}

void Socket::connect(const std::wstring &server){
	BOOL fSuccess = false;

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

void Socket::stopReadWait(){
	d->running = false;
	SetEvent(d->readEvent);
}

std::wstring Socket::read() {
	return read(INFINITE);
}

std::wstring Socket::read(int timeout){
	wchar_t buf[BUFSIZE];
	int result;
	DWORD cbBytesRead = 0;
	OVERLAPPED oOverlap; 
	
	memset(&oOverlap, 0, sizeof(OVERLAPPED));

	d->readEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	oOverlap.hEvent = d->readEvent;
	BOOL fSuccess = false;

	memset(&buf, 0, BUFSIZE);

	ReadFile( d->hPipe, &buf, sizeof(wchar_t)*BUFSIZE, &cbBytesRead, &oOverlap);

	result = WaitForSingleObject(oOverlap.hEvent, timeout);
	
	if(!d->running){
		CancelIo(d->hPipe);
		CloseHandle(d->readEvent);
		return L"";
	}

	if (result == WAIT_TIMEOUT) {
		CancelIo(d->hPipe);
		CloseHandle(d->readEvent);
		return L"";
	}
	fSuccess = GetOverlappedResult(d->hPipe, &oOverlap, &cbBytesRead, false);
	if (!fSuccess) {
		DWORD err = GetLastError();
		if (err == ERROR_BROKEN_PIPE) {
			d->connected = false;
		}
		buf[0] = 0;
	}

	CancelIo(d->hPipe);
	CloseHandle(d->readEvent);
	return buf;
}

void Socket::write(const std::wstring &msg){
	
	OVERLAPPED oOverlap;
	DWORD bytesWritten = 0;
	int result;
	BOOL fSuccess;

	memset(&oOverlap, 0, sizeof(OVERLAPPED));

	HANDLE writeEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	oOverlap.hEvent = writeEvent;
	
	WriteFile(d->hPipe, msg.data(), (DWORD)msg.length()*sizeof(wchar_t), &bytesWritten, &oOverlap);

	result = WaitForSingleObject(writeEvent, 500);
	if (result == WAIT_TIMEOUT) {
		CancelIo(d->hPipe);
		CloseHandle(writeEvent);
		d->connected = false;
		return;
	}
	fSuccess = GetOverlappedResult(d->hPipe, &oOverlap, &bytesWritten, false);
	CloseHandle(writeEvent);
	if (!fSuccess) {
		CancelIo(d->hPipe);
		d->connected = false;
		return;	
	}
}

bool Socket::isConnected(){
	return d->connected;
}