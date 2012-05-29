

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <xPL.h>
#include <telldus-core.h>

#define TELLDUS_VERSION "1.0"
#define INSTANCE_MAX 16

static xPL_ServicePtr telldusService = NULL;
static Bool daemonMode = TRUE;
char hostname[INSTANCE_MAX];

void sendDevInfo(xPL_MessagePtr msg);
void sendDevList(xPL_MessagePtr msg);
void sendGatewayInfo();
void sendNetInfo(xPL_MessagePtr msg);
void sendNetList();

void lightingCommandHandler(xPL_ServicePtr theService, xPL_MessagePtr theMessage, xPL_ObjectPtr userValue) {
	Bool found = FALSE;
	int deviceCount = 0, deviceId = 0, i, level = 0;
	
	if (!xPL_doesMessageNamedValueExist(theMessage, "command")) {
		return;
	} else if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(theMessage, "command"), "goto") != 0) {
		return;
	}

	if (!xPL_doesMessageNamedValueExist(theMessage, "network")) {
		return;
	} else if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(theMessage, "network"), "1") != 0) {
		return;
	}

	if (!xPL_doesMessageNamedValueExist(theMessage, "device")) {
		return;
	} else {
		/* Loop the devices to see it realy exists */
		deviceCount = tdGetNumberOfDevices();

		xPL_strToInt(xPL_getMessageNamedValue(theMessage, "device"), &deviceId);
		for( i = 0; i < deviceCount; ++i ) {
			if (tdGetDeviceId(i) == deviceId) {
				found = TRUE;
				break;
			}
		}
		if (found == FALSE) {
			return;
		}
	}

	if (!xPL_doesMessageNamedValueExist(theMessage, "level")) {
		return;
	} else {
		xPL_strToInt(xPL_getMessageNamedValue(theMessage, "level"), &level);
		if (level < 0 || level > 100) {
			return;
		}
		level = (float)level * 255.0 / 100.0;
	}

	if (level > 0 && level < 255) {
		/* See if the device supports dim */
		if (!(tdMethods(deviceId, TELLSTICK_DIM) & TELLSTICK_DIM)) {
			/* Non dimmable device was dimmed */
			return;
		}
		tdDim(deviceId, (unsigned char)level);
	} else if (level == 255) {
		/* See if the device supports dim */
		if (!(tdMethods(deviceId, TELLSTICK_TURNON) & TELLSTICK_TURNON)) {
			/* Non dimmable device was dimmed */
			return;
		}
		tdTurnOn(deviceId);
	} else if (level == 0) {
		/* See if the device supports dim */
		if (!(tdMethods(deviceId, TELLSTICK_TURNOFF) & TELLSTICK_TURNOFF)) {
			/* Non dimmable device was dimmed */
			return;
		}
		tdTurnOff(deviceId);
	}

}

void lightingRequestHandler(xPL_ServicePtr theService, xPL_MessagePtr theMessage, xPL_ObjectPtr userValue) {

	if (!xPL_doesMessageNamedValueExist(theMessage, "request")) {
		return;
	}

	if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(theMessage, "request"), "devinfo") == 0) {
		sendDevInfo(theMessage);
	} else if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(theMessage, "request"), "devlist") == 0) {
		sendDevList(theMessage);
	} else if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(theMessage, "request"), "gateinfo") == 0) {
		sendGatewayInfo();
	} else if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(theMessage, "request"), "netinfo") == 0) {
		sendNetInfo(theMessage);
	} else if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(theMessage, "request"), "netlist") == 0) {
		sendNetList();
	} else {
/* 		fprintf(stdout, "Request: %s\n", xPL_getMessageNamedValue(theMessage, "request")); */
	}
}

void shutdownHandler(int onSignal) {
	xPL_setServiceEnabled(telldusService, FALSE);
	xPL_releaseService(telldusService);
	xPL_shutdown();
	exit(0);
}

void sendDevInfo(xPL_MessagePtr msg) {
	xPL_MessagePtr message = NULL;
	Bool found = FALSE, deviceCount = tdGetNumberOfDevices();
	int deviceId = 0, methods = 0, i = 0, lastSentCommand, level = 0;
	char *name, buffer[12], *value;

	xPL_strToInt(xPL_getMessageNamedValue(msg, "device"), &deviceId);
	for( i = 0; i < deviceCount; ++i ) {
		if (tdGetDeviceId(i) == deviceId) {
			found = TRUE;
			break;
		}
	}

	message = xPL_createBroadcastMessage(telldusService, xPL_MESSAGE_STATUS);
	xPL_setSchema(message, "lighting", "devinfo");

	xPL_setMessageNamedValue(message, "network", xPL_getMessageNamedValue(msg, "network"));
	xPL_setMessageNamedValue(message, "device", xPL_getMessageNamedValue(msg, "device"));

	if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(msg, "network"), "1") == 0 && found == TRUE) {
		lastSentCommand = tdLastSentCommand(deviceId, TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM);
		if (lastSentCommand == TELLSTICK_TURNON) {
			level = 100;
		} else if (lastSentCommand == TELLSTICK_DIM) {
			value = tdLastSentValue(deviceId);
			level = atoi(value);
			free(value);
 			level = round((float)level / 255.0 * 100.0);
			if (level > 100) {
				level = 100;
			} else if (level < 0) {
				level = 0;
			}
		} else {
			level = 0;
		}
		methods = tdMethods(deviceId, TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM);
		name = tdGetName(deviceId);
		sprintf(buffer, "1,%s,0,%i", (methods & TELLSTICK_DIM ? "true" : "false"), level);
		xPL_setMessageNamedValue(message, "status", "ok");
		xPL_setMessageNamedValue(message, "name", name );
		xPL_setMessageNamedValue(message, "report-on-manual", "false" );
		xPL_setMessageNamedValue(message, "channel-count", "1" );
		xPL_setMessageNamedValue(message, "primary-channel", "1" );
		xPL_setMessageNamedValue(message, "channel", buffer );
		xPL_setMessageNamedValue(message, "scene-count", "0" );

		free(name);
	} else {
		xPL_setMessageNamedValue(message, "status", "not-found");
	}

	xPL_sendMessage(message);

	xPL_releaseMessage(message);
}

void sendDevList(xPL_MessagePtr msg) {
	int deviceCount = 0, i;
	char deviceList[128];
	xPL_MessagePtr message = NULL;

	message = xPL_createBroadcastMessage(telldusService, xPL_MESSAGE_STATUS);
	xPL_setSchema(message, "lighting", "devlist");

	xPL_setMessageNamedValue(message, "network", xPL_getMessageNamedValue(msg, "network"));

	if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(msg, "network"), "1") == 0) {
		xPL_setMessageNamedValue(message, "status", "ok");
		deviceCount = tdGetNumberOfDevices();
		for( i = 0; i < deviceCount; ++i ) {
			if (i == 0) { /* First */
				strcpy(deviceList, xPL_intToStr(tdGetDeviceId(i)));
			} else {
				strcat(deviceList, ",");
				strcat(deviceList, xPL_intToStr(tdGetDeviceId(i)));
			}
		}

		xPL_setMessageNamedValue(message, "device-count", xPL_intToStr(deviceCount) );
		xPL_setMessageNamedValue(message, "device", deviceList );
	} else {
		xPL_setMessageNamedValue(message, "status", "not-found");
	}

	xPL_sendMessage(message);

	xPL_releaseMessage(message);
}

void sendGatewayInfo() {
	xPL_MessagePtr message = NULL;

	message = xPL_createBroadcastMessage(telldusService, xPL_MESSAGE_STATUS);
	xPL_setSchema(message, "lighting", "gateinfo");

	xPL_setMessageNamedValue(message, "status", "ok");
	xPL_setMessageNamedValue(message, "protocol", "TELLDUS");
	xPL_setMessageNamedValue(message, "description", "xPL to Telldus TellStick gateway");
	xPL_setMessageNamedValue(message, "version", TELLDUS_VERSION);
	xPL_setMessageNamedValue(message, "author", "Telldus Technologies AB");
	xPL_setMessageNamedValue(message, "info-url", "http://www.telldus.se");
	xPL_setMessageNamedValue(message, "net-count", "1");
	xPL_setMessageNamedValue(message, "preferred-net", "1");
	xPL_setMessageNamedValue(message, "scenes-ok", "false");
	xPL_setMessageNamedValue(message, "channels-ok", "false");
	xPL_setMessageNamedValue(message, "fade-rate-ok", "false");

	/* Broadcast the message */
	xPL_sendMessage(message);

	xPL_releaseMessage(message);
}

void sendNetInfo(xPL_MessagePtr msg) {
	xPL_MessagePtr message = NULL;

	message = xPL_createBroadcastMessage(telldusService, xPL_MESSAGE_STATUS);
	xPL_setSchema(message, "lighting", "netinfo");

	xPL_setMessageNamedValue(message, "network", xPL_getMessageNamedValue(msg, "network"));

	if (xPL_strcmpIgnoreCase(xPL_getMessageNamedValue(msg, "network"), "1") == 0) {
		xPL_setMessageNamedValue(message, "status", "ok");
		xPL_setMessageNamedValue(message, "name", hostname );
		xPL_setMessageNamedValue(message, "device-count", xPL_intToStr(tdGetNumberOfDevices()) );
		xPL_setMessageNamedValue(message, "scene-count", "0" );
	} else {
		xPL_setMessageNamedValue(message, "status", "not-found");
	}

	xPL_sendMessage(message);

	xPL_releaseMessage(message);
}

void sendNetList() {
	xPL_MessagePtr message = NULL;

	message = xPL_createBroadcastMessage(telldusService, xPL_MESSAGE_STATUS);
	xPL_setSchema(message, "lighting", "netlist");

	xPL_setMessageNamedValue(message, "status", "ok");
	xPL_setMessageNamedValue(message, "network", "1");

	/* Broadcast the message */
	xPL_sendMessage(message);

	xPL_releaseMessage(message);
}

void sendGatewayReadyMessage() {
	xPL_MessagePtr gatewayReadyMessage = NULL;
	/* Create a message to send */
	gatewayReadyMessage = xPL_createBroadcastMessage(telldusService, xPL_MESSAGE_TRIGGER);
	xPL_setSchema(gatewayReadyMessage, "lighting", "gateway");

	/* Install the value and send the message */
	xPL_setMessageNamedValue(gatewayReadyMessage, "report", "gateway-ready");

	/* Broadcast the message */
	xPL_sendMessage(gatewayReadyMessage);

	xPL_releaseMessage(gatewayReadyMessage);
}

/* parseCmdLine will handles command line switches.  Valid switches are: */
/*  -interface x - set interface to use */
/*  -xpldebug - set debugging and enable xPL debugging */
/*  -nodaemon - Dosn't disconnect from the console */
static Bool parseCmdLine( int *argc, char *argv[]) {
	int swptr;
	int newcnt = 0;

	/* Handle each item of the command line.  If it starts with a '-', then */
	/* process it as a switch.  If not, then copy it to a new position in   */
	/* the argv list and up the new parm counter.                           */
	for(swptr = 0; swptr < *argc; swptr++) {
		/* If it doesn't begin with a '-', it's not a switch. */
		if (argv[swptr][0] != '-') {
			if (swptr != newcnt) argv[++newcnt] = argv[swptr];
		}	else {
			/* Check for daemon mode */
			if (!strcmp(argv[swptr],"-nodaemon")) {
				daemonMode = FALSE;
				continue;
			}

			/* Anything left is unknown */
			fprintf(stderr, "Unknown switch `%s'", argv[swptr] );
			return FALSE;
		}
	}

	/* Set in place the new argument count and exit */
	*argc = newcnt + 1;
	return TRUE;
}

/* Print command usage info out */
void printUsage(String ourName) {
	fprintf(stderr, "%s - Telldus TellStick xPL interface\n", ourName);
	fprintf(stderr, "Copyright (c) 2009, Telldus Technologies AB\n\n");
	fprintf(stderr, "Usage: %s [-xpldebug] [-nodaemon] [-ip x] [-interface x]\n", ourName);
	fprintf(stderr, "  -xpldebug -- enable xPLLib debugging messagaes\n");
	fprintf(stderr, "  -nodaemon -- don't detach -- run from the console\n");
	fprintf(stderr, "  -interface x -- Use interface named x (i.e. eth0) as network interface\n");
	fprintf(stderr, "  -ip x -- Bind to specified IP address for xPL\n");
}

void startServer() {
	/* Initialze lighting service */

	/* Create  a service for us */
	if (gethostname(hostname, INSTANCE_MAX) != 0) {
		fprintf(stderr, "Unable to retrieve the hostname");
		exit(1);
	}
	telldusService = xPL_createService("telldus", "core", hostname);
	xPL_setServiceVersion(telldusService, TELLDUS_VERSION);

	/* Add a responder */
	xPL_addServiceListener(telldusService, lightingRequestHandler, xPL_MESSAGE_COMMAND, "lighting", "request", NULL);
	xPL_addServiceListener(telldusService, lightingCommandHandler, xPL_MESSAGE_COMMAND, "lighting", "basic", NULL);

	/* Install signal traps for proper shutdown */
	signal(SIGTERM, shutdownHandler);
	signal(SIGINT, shutdownHandler);

	/* Enable the service */
	xPL_setServiceEnabled(telldusService, TRUE);
	sendGatewayReadyMessage();

	for (;;) {
		/* Let XPL run */
		xPL_processMessages(-1);
	}
}

int main(int argc, String argv[]) {

	/* Parse command line parms */
	if (!xPL_parseCommonArgs(&argc, argv, FALSE)) {
		exit(1);
	}

	/* Start xPL up */
	if (!xPL_initialize(xPL_getParsedConnectionType())) {
		fprintf(stderr, "Unable to start xPL");
		exit(1);
	}
	
	/* Parse Hub command arguments */
	if (!parseCmdLine(&argc, argv)) {
		printUsage(argv[0]);
		exit(1);
	}

	if (daemonMode) {
		switch (fork()) {
			case 0: /* child */
				/* No io in child mode */
				close(fileno(stdin));
				close(fileno(stdout));
				close(fileno(stderr));
				setpgrp();

				startServer();
				break;
			case -1: /* error */
				fprintf(stderr, "Unable to spawn daemon, %s (%d)\n", strerror(errno), errno);
				exit(1);
		}
	} else {
		startServer();
	}
	return 0;
}

