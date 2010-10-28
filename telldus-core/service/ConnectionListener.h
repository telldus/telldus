#ifndef CONNECTIONLISTENER_H
#define CONNECTIONLISTENER_H

#include <string>
#include "Thread.h"
#include "Event.h"

class Event;
namespace TelldusCore {
	class Socket;
};

class ConnectionListenerEventData : public EventDataBase {
public:
	TelldusCore::Socket *socket;
};

class ConnectionListener : public TelldusCore::Thread {
public:
	ConnectionListener(const std::wstring &name, EventRef waitEvent);
	virtual ~ConnectionListener(void);

protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONNECTIONLISTENER_H
