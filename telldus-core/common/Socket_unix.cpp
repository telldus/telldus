#include "Socket.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUFSIZE 512

using namespace TelldusCore;

int connectWrapper(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	connect(sockfd, addr, addrlen);
}

class Socket::PrivateData {
public:
	SOCKET_T socket;
};

Socket::Socket() {
	d = new PrivateData;
	d->socket = 0;
}

Socket::Socket(SOCKET_T socket)
{
	d = new PrivateData;
	d->socket = socket;
}

Socket::~Socket(void) {
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
}

std::wstring Socket::read() {
	char inbuf[BUFSIZE];
	memset(inbuf, '\0', sizeof(inbuf));
	
	size_t inlen = recv(d->socket, inbuf, BUFSIZE - 1, 0);

	std::string msg(inbuf);
	return std::wstring(msg.begin(), msg.end());
}

void Socket::write(const std::wstring &msg) {
	std::string newMsg(msg.begin(), msg.end());
	send(d->socket, newMsg.c_str(), newMsg.length(), 0);
}
