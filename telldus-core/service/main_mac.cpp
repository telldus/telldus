#include "TelldusMain.h"
#include "Log.h"
#include <signal.h>

TelldusMain tm;

void shutdownHandler(int onSignal) {
	Log::notice("Shutting down");
	tm.stop();
}

void sigpipeHandler(int onSignal) {
	Log::notice("SIGPIPE received");
}

int main(int argc, char **argv) {
	/* Install signal traps for proper shutdown */
	signal(SIGTERM, shutdownHandler);
	signal(SIGINT, shutdownHandler);
	signal(SIGPIPE, sigpipeHandler);

	Log::notice("telldusd started");
	tm.start();
	Log::notice("telldusd stopped gracefully");

	Log::destroy();
	return 0;
}
