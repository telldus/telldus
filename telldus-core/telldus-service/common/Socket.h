#ifndef SOCKET_H
#define SOCKET_H

#include "Message.h"

#define BUFSIZE 512

#ifdef _WINDOWS
	#include <windows.h>
	typedef HANDLE SOCKET_T;
#else
	typedef unsigned int SOCKET_T;
#endif

class Socket {
public:
	Socket(SOCKET_T socket);
	Socket();
	~Socket(void);

	void connectToServer(const std::string &serverName);
	void disconnect();
	void write(const TelldusService::Message &msg);
	void writeOverlapped(const TelldusService::Message &msg);
	std::string readWriteOverlapped(const TelldusService::Message &msg);
	std::string readOverlapped();
	std::string read();

	bool connected() const;

private:
	class PrivateData;
	PrivateData *d;
};

#endif //SOCKET_H
