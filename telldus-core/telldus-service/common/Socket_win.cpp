#include "Socket.h"
#include <windows.h>
//#include "TelldusCore.h"

class Socket::PrivateData {
public:
	SOCKET_T hPipe;
	HANDLE readEvent;
	bool connected;
};

Socket::Socket(SOCKET_T socket)
{
	d = new PrivateData;
	d->hPipe = socket;
	d->readEvent = INVALID_HANDLE_VALUE;
	d->connected = true;
}

Socket::Socket() {
	d = new PrivateData;
	d->hPipe = INVALID_HANDLE_VALUE;
	d->readEvent = INVALID_HANDLE_VALUE;
	d->connected = false;
}

Socket::~Socket(void) {
	if (d->connected) {
		disconnect();
	}
	if (d->hPipe != INVALID_HANDLE_VALUE) {
		/*TelldusCore::logMessage("Flush file");
		//FlushFileBuffers(d->hPipe);
		TelldusCore::logMessage("Cancel Sync");
		CancelSynchronousIo(d->thread);
		TelldusCore::logMessage("Disconnect named pipe");*/
		//DisconnectNamedPipe(d->hPipe);
		//TelldusCore::logMessage("CloseHandle");
		CloseHandle(d->hPipe); 
		//TelldusCore::logMessage("Done");
	}
	delete d;
}

void Socket::connectToServer(const std::string &server) {
	DWORD dwMode;
	bool  fSuccess = false;

	//Convert our std::string to std::wstring since we build agains win32 with unicode support
	std::string strName = "\\\\.\\pipe\\" + server;
	std::wstring name(strName.begin(), strName.end());
	d->hPipe = CreateFile(
		name.c_str(),           // pipe name 
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
	
	dwMode = PIPE_READMODE_MESSAGE; 
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

void Socket::disconnect() {
	d->connected = false;
	if (d->readEvent != INVALID_HANDLE_VALUE) {
		SetEvent(d->readEvent);
	}
}

void Socket::write(const TelldusService::Message &msg) {
	DWORD bytesWritten = 0;
	if (WriteFile(d->hPipe, msg.data(), (DWORD)msg.length(), &bytesWritten, NULL)) {
		FlushFileBuffers(d->hPipe);
	} else {
		d->connected = false;
	}
}

void Socket::writeOverlapped(const TelldusService::Message &msg) {
	OVERLAPPED oOverlap; 
	DWORD bytesWritten = 0;
	int result;
	bool fSuccess;

	oOverlap.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	WriteFile(d->hPipe, msg.data(), (DWORD)msg.length(), &bytesWritten, &oOverlap);

	result = WaitForSingleObject(oOverlap.hEvent, 10000);
	if (result == WAIT_TIMEOUT) {
		CloseHandle(oOverlap.hEvent);
		d->connected = false;
		return;
	}
	fSuccess = GetOverlappedResult(d->hPipe, &oOverlap, &bytesWritten, false);
	CloseHandle(oOverlap.hEvent);
	if (!fSuccess) {
		d->connected = false;
		return;
	}
}

std::string Socket::readOverlapped(int timeout) {
	char buf[BUFSIZE];
	int result;
	DWORD cbBytesRead = 0;
	OVERLAPPED oOverlap; 
	d->readEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	oOverlap.hEvent = d->readEvent;
	bool fSuccess = false;

	memset(&buf, 0, BUFSIZE);

	if (!d->connected) {
		return "";
	}

	ReadFile( d->hPipe, &buf, sizeof(char)*BUFSIZE, &cbBytesRead, &oOverlap);

	result = WaitForSingleObject(oOverlap.hEvent, timeout);
	if (result == WAIT_TIMEOUT) {
		CloseHandle(d->readEvent);
		d->readEvent = INVALID_HANDLE_VALUE;
		return "";
	}
	fSuccess = GetOverlappedResult(d->hPipe, &oOverlap, &cbBytesRead, false);
	if (!fSuccess) {
		DWORD err = GetLastError();
		if (err == ERROR_BROKEN_PIPE) {
			d->connected = false;
		}
		buf[0] = 0;
	}
	CloseHandle(d->readEvent);
	d->readEvent = INVALID_HANDLE_VALUE;
	return buf;
}

std::string Socket::readWriteOverlapped(const TelldusService::Message &msg) {
	int result;
	bool fSuccess = false;
	char buf[BUFSIZE];
	DWORD cbBytesRead = 0;
	OVERLAPPED oOverlap; 
	oOverlap.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

	char *temp = new char[msg.size()+1];
	strcpy_s(temp, msg.size()+1, msg.data());

	TransactNamedPipe( d->hPipe, temp, (DWORD)msg.length(), &buf, sizeof(char)*BUFSIZE, &cbBytesRead, &oOverlap );

	result = WaitForSingleObject(oOverlap.hEvent, 10000);
	delete temp;
	if (result == WAIT_TIMEOUT) {
		CloseHandle(oOverlap.hEvent);
		return "";
	}
	fSuccess = GetOverlappedResult(d->hPipe, &oOverlap, &cbBytesRead, false);
	CloseHandle(oOverlap.hEvent);
	if (!fSuccess) {
		return "";
	}
	return buf;
}

std::string Socket::read() {
	return this->readOverlapped(INFINITE);

	char buf[BUFSIZE];
	DWORD cbBytesRead = 0;
	bool fSuccess = false;

	//TelldusCore::logMessage("= Start read");

	//If we have a connection from a client this function will never complete
	//So while the is connected clients this service cannot exit properly.
	//We should find a way to make this function fail in the destructor to this
	//class.
	fSuccess = ReadFile( 
		d->hPipe,        // handle to pipe 
		&buf,    // buffer to receive data 
		BUFSIZE*sizeof(char), // size of buffer 
		&cbBytesRead, // number of bytes read 
		NULL);

	if (fSuccess) {
		//TelldusCore::logMessage(QString("= Done read: %1").arg(buf));

		return buf;
	} else {
		if (cbBytesRead == 0) {
			int err =  GetLastError();
			if (err == ERROR_BROKEN_PIPE) {
				//TelldusCore::logMessage("Broken Pipe");
			} else if (err == ERROR_INVALID_HANDLE) {
				//TelldusCore::logMessage("Invalid handle");
			} else {
				//TelldusCore::logMessage("Unknown error");
			}
		}
		//TelldusCore::logMessage(QString("= Failed read %1").arg(GetLastError()));
		d->connected = false;
	}
	return "";
}

bool Socket::connected() const {
	return d->connected;
}