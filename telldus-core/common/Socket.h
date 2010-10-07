#ifndef SOCKET_H
#define SOCKET_H

#include <string>

class Socket
{
public:
	Socket(HANDLE hPipe);
	virtual ~Socket(void);

	std::wstring read();
	
private:
	class PrivateData;
	PrivateData *d;
};

#endif //SOCKET_H
