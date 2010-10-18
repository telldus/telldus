#include "Client.h"
#include "Socket.h"

using namespace TelldusCore;

class Client::PrivateData {
public:
	Socket eventSocket;
};

Client::Client() {
	d = new PrivateData;
	d->eventSocket.connect(L"TelldusEvents");
}

Client::~Client(void) {
	delete d;
}
