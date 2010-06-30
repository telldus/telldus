#include "Pipe.h"
#include "Socket.h"

#include <sys/socket.h>
#include <sys/un.h>

#include <QDebug>

//using namespace TelldusService;

inline int listenWrapper(int sockfd, int backlog) { //To not collide with Pipe::listen
	return listen(sockfd, backlog);
}

class PipePrivate {
public:
	SOCKET_T socket;
	QString name;
};

Pipe::Pipe(QObject *parent)
	:QThread(parent)
{
	d = new PipePrivate;
	d->socket = -1;
}

Pipe::~Pipe(void) {
	qDebug() << "Closing pipe" << d->name;
	close(d->socket);
	qDebug() << "Waiting!";
	this->wait();
	delete d;
}

void Pipe::listen(const QString &name) {
	d->name = QString("/tmp/%1").arg(name);
	this->start();
}

void Pipe::run() {
	SOCKET_T s;
	struct sockaddr_un remote, name;
	socklen_t len;
	
	d->socket = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (d->socket < 0) {
		return;
	}
	name.sun_family = AF_LOCAL;
	memset(name.sun_path, '\0', sizeof(name.sun_path));
	strncpy(name.sun_path, d->name.toLocal8Bit(), sizeof(name.sun_path));
	unlink(name.sun_path);
	int size = SUN_LEN(&name);
	bind(d->socket, (struct sockaddr *)&name, size);
	qDebug() << "Running" << d->name;
	listenWrapper(d->socket, 5);
	qDebug() << "Listen";
	len = sizeof(struct sockaddr_un);
	while(1) {
		qDebug() << "Accepting" << d->name;
		s = accept(d->socket, (struct sockaddr *)&remote, &len);
		qDebug() << "Accepted" << d->name;
		emit newConnection(new Socket(s));
	}
}
