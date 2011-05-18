#include <unistd.h>
#include <stdio.h>
#include <telldus-core.h>

class Events {
public:
	Events();
	~Events();
	void deviceEvent(int deviceId, int method, const char *data);
	
	static void deviceEventCallback(int deviceId, int method, const char *data, int callbackId, void *context);

private:
	int callbackId;
};

Events::Events() {
	callbackId = tdRegisterDeviceEvent( reinterpret_cast<TDDeviceEvent>(&Events::deviceEventCallback), this );
}

Events::~Events() {
	tdUnregisterCallback(callbackId);
}

void Events::deviceEvent(int deviceId, int method, const char *data) {
	if (method == TELLSTICK_TURNON) {
		printf("ON event from device %i\n", deviceId);
	} else if (method == TELLSTICK_TURNOFF) {
		printf("OFF event from device %i\n", deviceId);
	} else {
		printf("Unknown event from device %i\n", deviceId);
	}
}

void Events::deviceEventCallback(int deviceId, int method, const char *data, int callbackId, void *context) {
	Events *e = reinterpret_cast<Events *>(context);
	if (e) {
		/** Please note!
		 * We are here in another thread than the main. Some measures to syncronize
		 * this must be taken!
		 **/
		e->deviceEvent(deviceId, method, data);
	}
}

int main(void) {
	tdInit();

	Events ev;

	//Our own simple eventloop
	while(1) {
		sleep(100);
	}

	tdClose();

	return 0;
}
