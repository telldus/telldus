#include "Socket.h"

#include "Mutex.h"
#include "Strings.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <math.h>

#define BUFSIZE 512

using namespace TelldusCore;

int connectWrapper(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	return connect(sockfd, addr, addrlen);
}

class Socket::PrivateData {
public:
	SOCKET_T socket;
	bool connected;
	fd_set infds;
	Mutex mutex;
};

Socket::Socket() {
	d = new PrivateData;
	d->socket = 0;
	d->connected = false;
	FD_ZERO(&d->infds);
}

Socket::Socket(SOCKET_T socket)
{
	d = new PrivateData;
	d->socket = socket;
	FD_ZERO(&d->infds);
	d->connected = true;
}

Socket::~Socket(void) {
	if(d->socket){
		close(d->socket);
	}
	delete d;
}

void Socket::connect(const std::wstring &server) {
	struct sockaddr_un remote;
	socklen_t len;

	if ((d->socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		return;
	}
	std::string name = "/tmp/" + std::string(server.begin(), server.end());
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, name.c_str());

	len = SUN_LEN(&remote);
	if (connectWrapper(d->socket, (struct sockaddr *)&remote, len) == -1) {
		return;
	}

	TelldusCore::MutexLocker locker(&d->mutex);
	d->connected = true;
}

bool Socket::isConnected(){
	TelldusCore::MutexLocker locker(&d->mutex);
	return d->connected;
}

std::wstring Socket::read() {
	return this->read(0);
}

std::wstring Socket::read(int timeout) {
	struct timeval tv;
	char inbuf[BUFSIZE];
	memset(inbuf, '\0', sizeof(inbuf));

	FD_SET(d->socket, &d->infds);
	while(isConnected()) {
		tv.tv_sec = floor(timeout / 1000.0);
		tv.tv_usec = timeout % 1000;

		int response = select(d->socket+1, &d->infds, NULL, NULL, &tv);
		if (response == 0 && timeout > 0) {
			return L"";
		} else if (response <= 0) {
			FD_SET(d->socket, &d->infds);
			continue;
		}

		int received = recv(d->socket, inbuf, BUFSIZE - 1, 0);
		if (received <= 0) {
			TelldusCore::MutexLocker locker(&d->mutex);
			d->connected = false;
		}
		break;
	}

	std::string msg(inbuf);
	return TelldusCore::charToWstring(msg.c_str());
}

void Socket::stopReadWait(){
	TelldusCore::MutexLocker locker(&d->mutex);
	d->connected = false;
}

void Socket::write(const std::wstring &msg) {
	std::string newMsg(TelldusCore::wideToString(msg));
	int sent = send(d->socket, newMsg.c_str(), newMsg.length(), 0);
	if (sent < 0) {
		TelldusCore::MutexLocker locker(&d->mutex);
		d->connected = false;
	}
}
