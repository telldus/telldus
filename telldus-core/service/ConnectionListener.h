#ifndef CONNECTIONLISTENER_H
#define CONNECTIONLISTENER_H

#include <string>

class Event;
namespace TelldusCore {
	class Socket;
};

class ConnectionListener
{
public:
	ConnectionListener(const std::wstring &name);
	virtual ~ConnectionListener(void);
	void listen(Event *waitEvent);
	TelldusCore::Socket *retrieveClientSocket();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONNECTIONLISTENER_H
