#ifndef CLIENTMESSAGEHANDLER_H
#define CLIENTMESSAGEHANDLER_H

#include <string>
#include "Thread.h"

class ClientMessageHandler : public TelldusCore::Thread
{
public:
	ClientMessageHandler(const std::wstring &clientMessage);
	~ClientMessageHandler(void);

protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CLIENTMESSAGEHANDLER_H