#ifndef EVENTUPDATEMANAGER_H
#define EVENTUPDATEMANAGER_H

#include "Thread.h"
#include "Event.h"

class EventUpdateData : public EventDataBase {
public:
	int eventDeviceChanges;
	int eventMethod;
	int eventChangeType;
	int deviceType;
	int deviceId;
};

class EventUpdateManager  : public TelldusCore::Thread
{
public:
	EventUpdateManager(void);
	~EventUpdateManager(void);

	Event *retrieveUpdateEvent();
	Event *retrieveClientConnectEvent();

protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
	void sendMessageToClients(EventUpdateData *data);
};

#endif //EVENTUPDATEMANAGER_H