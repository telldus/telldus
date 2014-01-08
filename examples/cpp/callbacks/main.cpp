#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <telldus-core.h>

bool running;

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

void signalHandler(int sig) {
	if (sig == SIGINT) {
		printf("Shutting down\n");
		running = false;
	}
}

void run() {
	Events ev;

	running = true;

	printf("Listening for events, press control-c to quit...\n");
	//Our own simple eventloop
	while(running) {
		sleep(1);
	}
}

int main(void) {
	tdInit();

	signal(SIGINT, signalHandler);
	run();

	tdClose();

	return 0;
}
