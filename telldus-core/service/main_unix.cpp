
#include <signal.h>

void shutdownHandler(int onSignal) {
}

int main(int argc, char **argv) {
	/* Install signal traps for proper shutdown */
	signal(SIGTERM, shutdownHandler);
	signal(SIGINT, shutdownHandler);	
}
