#include "ConnectionListener.h"


class ConnectionListener::PrivateData {
public:
};

ConnectionListener::ConnectionListener(const std::wstring &name, Event *waitEvent)
{
	d = new PrivateData;
}

ConnectionListener::~ConnectionListener(void) {
	delete d;
}

void ConnectionListener::run(){
	
}

