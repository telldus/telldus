#include "ConnectionListener.h"
#include "Socket.h"

#include <string>
#include <sys/socket.h>
#include <sys/un.h>

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

class ConnectionListener::PrivateData {
public:
	Event *waitEvent;
	std::string name;
	bool running;
};

ConnectionListener::ConnectionListener(const std::wstring &name, Event *waitEvent)
{
	d = new PrivateData;
	d->waitEvent = waitEvent;
	
	d->name = "/tmp/" + std::string(name.begin(), name.end());
	d->running = true;

	this->start();
}

ConnectionListener::~ConnectionListener(void) {
	d->running = false;
	this->wait();
	delete d->waitEvent;
	delete d;
}

void ConnectionListener::run(){
	struct timeval tv = { 0, 0 };

	//Timeout for select

	SOCKET_T serverSocket;
	struct sockaddr_un name;
	socklen_t len;
	serverSocket = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		return;
	}
	name.sun_family = AF_LOCAL;
	memset(name.sun_path, '\0', sizeof(name.sun_path));
	strncpy(name.sun_path, d->name.c_str(), sizeof(name.sun_path));
	unlink(name.sun_path);
	int size = SUN_LEN(&name);
	bind(serverSocket, (struct sockaddr *)&name, size);
	listen(serverSocket, 5);
	len = sizeof(struct sockaddr_un);

	fd_set infds;
	FD_ZERO(&infds);
	FD_SET(serverSocket, &infds);
	
	while(d->running) {
		tv.tv_sec = 5;

		int response = select(serverSocket+1, &infds, NULL, NULL, &tv);
		if (response == 0) {
			FD_SET(serverSocket, &infds);
			continue;
		} else if (response < 0 ) {
			printf("Select error occured: %i\n", response);
			continue;
		}
		//Make sure it is a new connection
		if (!FD_ISSET(serverSocket, &infds)) {
			printf("Not ours, error?\n");
			continue;
		}
		SOCKET_T clientSocket = accept(serverSocket, NULL, NULL);
		if(clientSocket == -1){
				printf("error %i\n", errno); 
		}

		ConnectionListenerEventData *data = new ConnectionListenerEventData();
		data->socket = new TelldusCore::Socket(clientSocket);
		d->waitEvent->signal(data);

	}
	close(serverSocket);
}

