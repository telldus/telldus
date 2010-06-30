#include "Socket.h"
#include <sys/un.h>
#include <sys/socket.h>
#include <errno.h>

// #include <QDebug>

//Not needed when the class Socket isn't derived from QObject anymore
int connectWrapper(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	connect(sockfd, addr, addrlen);
}

class Socket::PrivateData {
public:
	SOCKET_T socket;
	bool connected;
};

Socket::Socket(SOCKET_T socket)
{
	d = new PrivateData;
	d->socket = socket;
	d->connected = true;
}

Socket::Socket() {
	d = new PrivateData;
	d->socket = -1;
	d->connected = false;
}

Socket::~Socket(void) {
	if (d->connected) {
		disconnect();
	}
	delete d;
}

void Socket::connectToServer(const QString &server) {
	struct sockaddr_un remote;
	socklen_t len;
	//QString name = QString("/tmp/%1").arg(server);
	
	if ((d->socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		return;
	}
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, server.toLocal8Bit());
	len = SUN_LEN(&remote);

	if (connectWrapper(d->socket, (struct sockaddr *)&remote, len) == -1) {
		return;
	}
	d->connected = true;
}

void Socket::disconnect() {
	d->connected = false;
	close(d->socket);
	emit disconnected();
}

void Socket::write(const TelldusService::Message &msg) {
	send(d->socket, msg.c_str(), msg.length(), 0);
}

void Socket::writeOverlapped(const TelldusService::Message &msg) {
	this->write(msg);
}

std::string Socket::readOverlapped() {
	return this->read();
}

std::string Socket::read() {
	ssize_t len;
	char buf[BUFSIZE];
	
	len = recv(d->socket, buf, BUFSIZE, 0);
	if (len <= 0) {
		return "";
	}
	return buf;
}

bool Socket::connected() const {
	return d->connected;
}
