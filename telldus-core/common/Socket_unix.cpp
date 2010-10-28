#include "Socket.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>

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

	d->connected = true;
}

bool Socket::isConnected(){
	return d->connected;
}

std::wstring Socket::read(int) {
	return this->read();
}

std::wstring Socket::read() {
	struct timeval tv = { 1, 0 };
	char inbuf[BUFSIZE];
	memset(inbuf, '\0', sizeof(inbuf));

	FD_SET(d->socket, &d->infds);
	while(d->connected) {
		tv.tv_sec = 1;

		int response = select(d->socket+1, &d->infds, NULL, NULL, &tv);
		if (response <= 0) {
			FD_SET(d->socket, &d->infds);
			continue;
		}

		int received = recv(d->socket, inbuf, BUFSIZE - 1, 0);
		if (received <= 0) {
			d->connected = false;
		}
		break;
	}

	std::string msg(inbuf);
	return std::wstring(msg.begin(), msg.end());
}

void Socket::stopReadWait(){
	//TODO, lock?
	d->connected = false;
}

void Socket::write(const std::wstring &msg) {
	std::string newMsg(msg.begin(), msg.end());
	int sent = send(d->socket, newMsg.c_str(), newMsg.length(), 0);
	if (sent < 0) {
		d->connected = false;
	}
}
