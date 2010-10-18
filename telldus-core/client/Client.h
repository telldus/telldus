#ifndef CLIENT_H
#define CLIENT_H

#include "Message.h"

namespace TelldusCore {
	class Client{
	public:
		~Client(void);

		static Client *getInstance();
		static void close();

		static bool getBoolFromService(const Message &msg);
		static int getIntegerFromService(const Message &msg);
		static std::wstring getWStringFromService(const Message &msg);

	private:
		Client();
		static std::wstring sendToService(const Message &msg);

		class PrivateData;
		PrivateData *d;
		static Client *instance;
	};
}

#endif //CLIENT_H
