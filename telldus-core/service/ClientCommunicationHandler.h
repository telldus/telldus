#ifndef CLIENTCOMMUNICATIONHANDLER_H
#define CLIENTCOMMUNICATIONHANDLER_H

#include <string>
#include "Thread.h"
#include "Socket.h"
#include "Event.h"
#include "DeviceManager.h"
#include "ControllerManager.h"

class ClientCommunicationHandler : public TelldusCore::Thread
{
public:
	ClientCommunicationHandler();
	ClientCommunicationHandler(TelldusCore::Socket *clientSocket, EventRef event, DeviceManager *deviceManager, EventRef deviceUpdateEvent);
	~ClientCommunicationHandler(void);

	bool isDone();

protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
	void parseMessage(const std::wstring &clientMessage, int *intReturn, std::wstring *wstringReturn);
	void sendDeviceSignal(int deviceId, int eventDeviceChanges, int eventChangeType);
};

#endif //CLIENTCOMMUNICATIONHANDLER_H
