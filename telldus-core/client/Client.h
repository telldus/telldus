#ifndef CLIENT_H
#define CLIENT_H

#include "Message.h"

namespace TelldusCore {
	class Client{
	public:
		~Client(void);

		static Client *getInstance();
		static void close();

		static int getIntegerFromService(const Message &msg);

	private:
		Client();

		class PrivateData;
		PrivateData *d;
		static Client *instance;
	};
}

#endif //CLIENT_H
