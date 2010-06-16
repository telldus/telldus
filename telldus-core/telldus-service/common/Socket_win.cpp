#include "Socket.h"
//#include "TelldusCore.h"

class SocketPrivate {
public:
	SOCKET_T hPipe;
	HANDLE event;
	bool connected;
};

Socket::Socket(SOCKET_T socket)
{
	d = new SocketPrivate;
	d->hPipe = socket;
	d->event = INVALID_HANDLE_VALUE;
	d->connected = true;
}

Socket::Socket() {
	d = new SocketPrivate;
	d->hPipe = INVALID_HANDLE_VALUE;
	d->event = INVALID_HANDLE_VALUE;
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
	if (d->event != INVALID_HANDLE_VALUE) {
		CloseHandle(d->event);
	}
	delete d;
}

void Socket::connectToServer(const QString &server) {
	DWORD dwMode;
	bool  fSuccess = false;

	d->event = CreateEvent( 
         NULL,    // default security attribute 
         TRUE,    // manual-reset event 
         TRUE,    // initial state = signaled 
         NULL);   // unnamed event object 

	if (d->event == NULL) {
		return;
	}

	QString name = QString("\\\\.\\pipe\\%1").arg(server);
	d->hPipe = CreateFile(
		(const wchar_t *)name.utf16(),   // pipe name 
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
      d->hPipe,    // pipe handle 
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
	if (d->event != INVALID_HANDLE_VALUE) {
		SetEvent(d->event);
	}
	emit disconnected();

}

void Socket::write(const TelldusService::Message &msg) {
	DWORD bytesWritten = 0;
	if (WriteFile(d->hPipe, msg.data(), msg.length(), &bytesWritten, NULL)) {
		//FlushFileBuffers(d->hPipe);
	} else {
		d->connected = false;
		emit disconnected();
	}
}

void Socket::writeOverlapped(const TelldusService::Message &msg) {
	OVERLAPPED oOverlap; 
	DWORD bytesWritten = 0;
	int result;
	bool fSuccess;

	oOverlap.hEvent = d->event;
	WriteFile(d->hPipe, msg.data(), msg.length(), &bytesWritten, &oOverlap);

	result = WaitForSingleObject(oOverlap.hEvent, 10000);
	if (result == WAIT_TIMEOUT) {
		d->connected = false;
		emit disconnected();
		return;
	}
	fSuccess = GetOverlappedResult(d->hPipe, &oOverlap, &bytesWritten, false);
	if (!fSuccess) {
		d->connected = false;
		emit disconnected();
		return;
	}
}

QByteArray Socket::readOverlapped() {
	char buf[BUFSIZE];
	int result;
	DWORD cbBytesRead = 0;
	OVERLAPPED oOverlap; 
	oOverlap.hEvent = d->event;
	bool fSuccess = false;

	memset(&buf, 0, BUFSIZE);

	if (!d->connected) {
		return "";
	}

	ReadFile( d->hPipe, &buf, sizeof(char)*BUFSIZE, &cbBytesRead, &oOverlap);

	result = WaitForSingleObject(oOverlap.hEvent, INFINITE);
	if (result == WAIT_TIMEOUT) {
		return "";
	}
	fSuccess = GetOverlappedResult(d->hPipe, &oOverlap, &cbBytesRead, false);
	if (!fSuccess) {
		return "";
	}
	return buf;
}

QByteArray Socket::read() {
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
		emit disconnected();
	}
	return "";
}

bool Socket::connected() const {
	return d->connected;
}