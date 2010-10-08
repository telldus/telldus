#ifndef CLIENTCOMMUNICATIONHANDLER_H
#define CLIENTCOMMUNICATIONHANDLER_H

#include <string>
#include "Thread.h"

class ClientCommunicationHandler : public TelldusCore::Thread
{
public:
	ClientCommunicationHandler(const std::wstring &clientMessage);
	~ClientCommunicationHandler(void);

protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CLIENTCOMMUNICATIONHANDLER_H