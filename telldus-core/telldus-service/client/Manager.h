#ifndef MANAGER_H
#define MANAGER_H

#include "Message.h"
#include <libtelldus-core/Thread.h>
#include <libtelldus-core/telldus-core.h>

class ManagerPrivate;

struct CallbackStruct {
	TDDeviceEvent event;
	int id;
	void *context;
};
struct DeviceChangeCallbackStruct {
	TDDeviceChangeEvent event;
	int id;
	void *context;
};
struct RawCallbackStruct {
	TDRawDeviceEvent event;
	int id;
	void *context;
};


class Manager : public TelldusCore::Thread {
public:
	~Manager(void);

	static Manager *getInstance();
	static void close();

	std::string send(const TelldusService::Message &msg, bool *success);
	std::string sendAndReceiveString(const TelldusService::Message &msg, bool *success);
	int sendAndReceiveInt(const TelldusService::Message &msg, bool *success);

	int numberOfDevices();
	std::string deviceName(int deviceId);

	int registerDeviceEvent( TDDeviceEvent eventFunction, void *context );
	int registerDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context );
	int registerRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context );
	bool unregisterCallback( int callbackId );

	static void logMessage( const std::string &message);

protected:
	void run();

private:
	Manager(void);
	void dataReceived(const std::string &msg);

	ManagerPrivate *d;
	static Manager *instance;
};

#endif //MANAGER_H
