#ifndef CONNECTIONLISTENER_H
#define CONNECTIONLISTENER_H

#include <string>
#include "Thread.h"

class Event;
namespace TelldusCore {
	class Socket;
};

class ConnectionListener : public TelldusCore::Thread {
public:
	ConnectionListener(const std::wstring &name, Event *waitEvent);
	virtual ~ConnectionListener(void);
	TelldusCore::Socket *retrieveClientSocket();

protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONNECTIONLISTENER_H
