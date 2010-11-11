#include "TelldusMain.h"
#include <signal.h>
#include <stdio.h>

TelldusMain tm;

void shutdownHandler(int onSignal) {
	printf("Shutting down\n");
	tm.stop();
}

void sigpipeHandler(int onSignal) {
	printf("SIGPIPE received\n");
}

int main(int argc, char **argv) {
	/* Install signal traps for proper shutdown */
	signal(SIGTERM, shutdownHandler);
	signal(SIGINT, shutdownHandler);
	signal(SIGPIPE, sigpipeHandler);

	tm.start();
	printf("telldusd stopped gracefully\n");
	return 0;
}
