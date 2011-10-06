#ifndef CLIENT_H
#define CLIENT_H

#include "Message.h"
#include "telldus-core.h"
#include "Thread.h"

namespace TelldusCore {
	class Client : public Thread
	{
	public:
		~Client(void);

		static Client *getInstance();
		static void close();

		void callbackDeviceEvent(int deviceId, int deviceState, const std::wstring &deviceStateValue);
		void callbackDeviceChangeEvent(int deviceId, int eventDeviceChanges, int eventChangeType);
		void callbackRawEvent(std::wstring command, int controllerId);
		void callbackSensorEvent(const std::wstring &protocol, const std::wstring &model, int id, int dataType, const std::wstring &value, int timestamp);
		int registerDeviceEvent( TDDeviceEvent eventFunction, void *context );
		int registerDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context );
		int registerRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context );
		int registerSensorEvent( TDSensorEvent eventFunction, void *context );
		void stopThread(void);
		bool unregisterCallback( int callbackId );

		int getSensor(char *protocol, int protocolLen, char *model, int modelLen, int *id, int *dataTypes);

		static bool getBoolFromService(const Message &msg);
		static int getIntegerFromService(const Message &msg);
		static std::wstring getWStringFromService(const Message &msg);

	protected:
			void run(void);

	private:
		Client();
		static std::wstring sendToService(const Message &msg);
		void cleanupCallbacks();

		class PrivateData;
		PrivateData *d;
		static Client *instance;
	};
}

#endif //CLIENT_H
