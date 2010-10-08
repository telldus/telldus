#include "Socket.h"

using namespace TelldusCore;

class Socket::PrivateData {
public:
};

Socket::Socket(SOCKET_T hPipe)
{
	d = new PrivateData;
}

Socket::~Socket(void){
	delete d;
}

std::wstring Socket::read() {
	return L"";
}
