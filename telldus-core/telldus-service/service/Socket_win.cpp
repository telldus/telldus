#include "Socket.h"
#include "TelldusCore.h"
#include <QMutex>

class SocketPrivate {
public:
	SOCKET_T hPipe;
	bool connected;
};

Socket::Socket(SOCKET_T socket)
{
	d = new SocketPrivate;
	d->hPipe = socket;
	d->connected = true;
}

Socket::~Socket(void) {
	/*TelldusCore::logMessage("Flush file");
	//FlushFileBuffers(d->hPipe);
	TelldusCore::logMessage("Cancel Sync");
	CancelSynchronousIo(d->thread);
	TelldusCore::logMessage("Disconnect named pipe");
	//DisconnectNamedPipe(d->hPipe); */
	TelldusCore::logMessage("CloseHandle");
	CloseHandle(d->hPipe); 
	TelldusCore::logMessage("Done");
	delete d;
}

void Socket::write(const TelldusService::Message &msg) {
	DWORD bytesWritten = 0;
	TelldusCore::logMessage(QString("Sending: %1").arg(QString(msg)));
	WriteFile(d->hPipe, msg.data(), msg.length(), &bytesWritten, NULL);
	TelldusCore::logMessage(QString("Done sending"));
	FlushFileBuffers(d->hPipe);
}

QByteArray Socket::read() {
	char buf[BUFSIZE];
	DWORD cbBytesRead = 0;

	TelldusCore::logMessage("= Start read");

	//If we have a connection from a client this function will never complete
	//So while the is connected clients this service cannot exit properly.
	//We should find a way to make this function fail in the destructor to this
	//class.
	bool fSuccess = ReadFile( 
		d->hPipe,        // handle to pipe 
		&buf,    // buffer to receive data 
		BUFSIZE*sizeof(char), // size of buffer 
		&cbBytesRead, // number of bytes read 
		NULL);        // not overlapped I/O 

	if (fSuccess) {
		TelldusCore::logMessage(QString("= Done read: %1").arg(buf));
		return buf;
	} else {
		if (cbBytesRead == 0) {
			int err =  GetLastError();
			if (err == ERROR_BROKEN_PIPE) {
				TelldusCore::logMessage("Broken Pipe");
			} else if (err == ERROR_INVALID_HANDLE) {
				TelldusCore::logMessage("Invalid handle");
			} else {
				TelldusCore::logMessage("Unknown error");
			}
		}
		TelldusCore::logMessage(QString("= Failed read %1").arg(GetLastError()));
		d->connected = false;
	}
	return "";
}

bool Socket::connected() const {
	return d->connected;
}