#ifndef SOCKET_H
#define SOCKET_H

#include <string>

#ifdef _WINDOWS
	#include <windows.h>
	typedef HANDLE SOCKET;
#else
	typedef unsigned int SOCKET;
#endif

class Socket
{
public:
	Socket(SOCKET hPipe);
	virtual ~Socket(void);

	std::wstring read();
	
private:
	class PrivateData;
	PrivateData *d;
};

#endif //SOCKET_H
