#include "TelldusMain.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

#include "Settings.h"
#include "Strings.h"

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
		if (strcmp(argv[i], "--help") == 0) {
			printf("Telldus TellStick background service\n\nStart with --nodaemon to not run as daemon\n\n");
			printf("Report bugs to <info.tech@telldus.com>\n");
			exit(EXIT_SUCCESS);
		}
		if (strcmp(argv[i], "--version") == 0) {
			printf("telldusd " VERSION "\n\n");
			printf("Copyright (C) 2011 Telldus Technologies AB\n\n");
			printf("Written by Micke Prag <micke.prag@telldus.se>\n");
			exit(EXIT_SUCCESS);
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
				syslog(LOG_ERR, "Could not open pid file %s: %s", PID_FILE, strerror(errno));
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
	}

	setlogmask(LOG_UPTO(LOG_INFO));
	openlog(DAEMON_NAME, LOG_CONS, LOG_USER);

	syslog(LOG_NOTICE, "%s daemon starting up", DAEMON_NAME);

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

	/* Reduce our permissions (change user and group) */
	if (getuid() == 0 || geteuid() == 0) {
		Settings settings;
		std::string user = TelldusCore::wideToString(settings.getSetting(L"user"));
		std::string group = TelldusCore::wideToString(settings.getSetting(L"group"));

		struct group *grp = getgrnam(group.c_str());
		if (grp) {
			setgid(grp->gr_gid);
		} else {
			syslog(LOG_WARNING, "Group %s could not be found", group.c_str());
			exit(EXIT_FAILURE);
		}
		struct passwd *pw = getpwnam(user.c_str());
		if (pw) {
			setuid( pw->pw_uid );
		} else {
			syslog(LOG_WARNING, "User %s could not be found", user.c_str());
			exit(EXIT_FAILURE);
		}
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}

	/* Install signal traps for proper shutdown */
	signal(SIGTERM, signalHandler);
	signal(SIGINT,  signalHandler);
	signal(SIGPIPE, signalHandler);

	tm.start();

	syslog(LOG_NOTICE, "%s daemon exited", DAEMON_NAME);
	exit(EXIT_SUCCESS);
}
