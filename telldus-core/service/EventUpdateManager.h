#ifndef EVENTUPDATEMANAGER_H
#define EVENTUPDATEMANAGER_H

class EventUpdateManager {
public:
	EventUpdateManager(void);
	~EventUpdateManager(void);

	void sendUpdateMessage(int eventDeviceChanges, int eventChangeType, int eventMethod, int deviceType, int deviceId);

private:
	class PrivateData;
	PrivateData *d;
};

#endif //EVENTUPDATEMANAGER_H