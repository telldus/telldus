#ifndef EVENTUPDATEMANAGER_H
#define EVENTUPDATEMANAGER_H

#include "Thread.h"
#include "Event.h"

class EventUpdateData : public EventDataBase {
public:
	std::wstring messageType;
	int controllerId;
	int deviceId;
	int eventChangeType;
	int eventDeviceChanges;
	int eventState;
	std::wstring eventValue;

	//Sensor event
	std::wstring protocol;
	std::wstring model;
	int sensorId;
	int dataType;
	std::wstring value;
	int timestamp;
};

class EventUpdateManager  : public TelldusCore::Thread
{
public:
	EventUpdateManager(void);
	~EventUpdateManager(void);

	EventRef retrieveUpdateEvent();
	EventRef retrieveClientConnectEvent();

protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
	void sendMessageToClients(EventUpdateData *data);
};

#endif //EVENTUPDATEMANAGER_H
