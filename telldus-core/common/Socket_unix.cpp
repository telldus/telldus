#include "Socket.h"

using namespace TelldusCore;

class Socket::PrivateData {
public:
};

Socket::Socket() {
	d = new PrivateData;
}

Socket::Socket(SOCKET_T hPipe)
{
	d = new PrivateData;
}

Socket::~Socket(void) {
	delete d;
}

void Socket::connect(const std::wstring &server) {
}

std::wstring Socket::read() {
	return L"";
}

void Socket::write(const std::wstring &msg) {
	
}
