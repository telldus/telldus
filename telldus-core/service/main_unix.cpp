#include "TelldusMain.h"
#include <signal.h>
#include <stdio.h>

TelldusMain tm;

void shutdownHandler(int onSignal) {
	printf("Shutting down\n");
	tm.stop();
}

int main(int argc, char **argv) {
	/* Install signal traps for proper shutdown */
	signal(SIGTERM, shutdownHandler);
	signal(SIGINT, shutdownHandler);

	tm.start();
	printf("telldusd stopped gracefully\n");
	return 0;
}
