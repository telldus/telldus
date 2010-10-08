#include "ConnectionListener.h"


class ConnectionListener::PrivateData {
public:
};

ConnectionListener::ConnectionListener(const std::wstring &name)
{
	d = new PrivateData;
}

ConnectionListener::~ConnectionListener(void) {
	delete d;
}

void ConnectionListener::listen(Event *waitEvent){
	
}

TelldusCore::Socket *ConnectionListener::retrieveClientSocket(){
	
}
