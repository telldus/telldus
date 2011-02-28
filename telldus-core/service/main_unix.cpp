#include "TelldusMain.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>

#define DAEMON_NAME "telldusd"
#define PID_FILE "/var/run/" DAEMON_NAME ".pid"

TelldusMain tm;

void signalHandler(int sig) {
	switch(sig) {
		case SIGHUP:
			syslog(LOG_WARNING, "Received SIGHUP signal.");
			break;
		case SIGTERM:
		case SIGINT:
			syslog(LOG_WARNING, "Received SIGTERM or SIGINT signal.");
			syslog(LOG_WARNING, "Shutting down");
			tm.stop();
			break;
		case SIGPIPE:
			syslog(LOG_WARNING, "Received SIGPIPE signal.");
			break;
		default:
			syslog(LOG_WARNING, "Unhandled signal (%d) %s", sig, strsignal(sig));
			break;
	}
}

int main(int argc, char **argv) {
	pid_t pid, sid;
	FILE *fd;
	bool deamonize = true;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--nodaemon") == 0) {
			deamonize = false;
		}
	}

	if (deamonize) {
		pid = fork();
		if (pid < 0) {
			exit(EXIT_FAILURE);
		}
		if (pid > 0) {
			//We are the parent
			//Let the parent store the clients pid,
			//This way anyone starting the daemon can read the pidfile immediately

			// Record the pid
			fd = fopen(PID_FILE,"w");
			if (fd) {
				fprintf(fd,"%d\n",pid);
				fclose(fd);
			} else {
				syslog(LOG_ERR, "Could not write pid file");
			}
			exit(EXIT_SUCCESS);
		}
	}

	setlogmask(LOG_UPTO(LOG_INFO));
	openlog(DAEMON_NAME, LOG_CONS, LOG_USER);

	syslog(LOG_INFO, "%s daemon starting up", DAEMON_NAME);

	if (deamonize) {
		/* Change the file mode mask */
		umask(0);

		sid = setsid();

		if (sid < 0) {
			//Something went wrong
			printf("Could not set sid\n");
			exit(EXIT_FAILURE);
		}

		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}

	//TODO: Reduce our permissions (change user)

	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}

	/* Install signal traps for proper shutdown */
	signal(SIGTERM, signalHandler);
	signal(SIGINT,  signalHandler);
	signal(SIGPIPE, signalHandler);

	tm.start();

	syslog(LOG_INFO, "%s daemon exited", DAEMON_NAME);
	exit(EXIT_SUCCESS);
}
