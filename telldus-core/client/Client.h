#ifndef CLIENT_H
#define CLIENT_H

#include "Message.h"
#include "telldus-core.h"

namespace TelldusCore {
	class Client{
	public:
		~Client(void);

		static Client *getInstance();
		static void close();

		int registerDeviceEvent( TDDeviceEvent eventFunction, void *context );
		int registerDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context );
		int registerRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context );
		bool unregisterCallback( int callbackId );

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
