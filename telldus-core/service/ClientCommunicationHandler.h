#ifndef CLIENTCOMMUNICATIONHANDLER_H
#define CLIENTCOMMUNICATIONHANDLER_H

#include <string>
#include "common/Thread.h"
#include "common/Socket.h"
#include "common/Event.h"
#include "service/DeviceManager.h"
#include "service/ControllerManager.h"

class ClientCommunicationHandler : public TelldusCore::Thread
{
public:
	ClientCommunicationHandler();
	ClientCommunicationHandler(
		TelldusCore::Socket *clientSocket,
		TelldusCore::EventRef event,
		DeviceManager *deviceManager,
		TelldusCore::EventRef deviceUpdateEvent,
		ControllerManager *controllerManager
	);
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
