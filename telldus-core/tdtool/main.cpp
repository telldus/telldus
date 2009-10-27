#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "../driver/libtelldus-core/telldus-core.h"

const int SUPPORTED_METHODS =
	TELLSTICK_TURNON |
	TELLSTICK_TURNOFF |
	TELLSTICK_BELL |
	TELLSTICK_DIM;

void print_usage( char *name ) {
	printf("Usage: %s [ options ]\n", name);
	printf("\n");
	printf("Options:\n");
	printf("         -[bdefhlnrv] [ --list ] [ --help ]\n");
	printf("                      [ --on device ] [ --off device ] [ --bell device ]\n");
	printf("                      [ --learn device ]\n");
	printf("                      [ --dimlevel level --dim device ]\n");
	printf("                      [ --raw input ]\n");
	printf("\n");
	printf("       --list (-l short option)\n");
	printf("             List currently configured devices.\n");
	printf("\n");
	printf("       --help (-h short option)\n");
	printf("             Shows this screen.\n");
	printf("\n");
	printf("       --on device (-n short option)\n");
	printf("             Turns on device. 'device' could either be an integer of the\n");
	printf("             device-id, or the name of the device.\n");
	printf("             Both device-id and name is outputed with the --list option\n");
	printf("\n");
	printf("       --off device (-f short option)\n");
	printf("             Turns off device. 'device' could either be an integer of the\n");
	printf("             device-id, or the name of the device.\n");
	printf("             Both device-id and name is outputed with the --list option\n");
	printf("\n");
	printf("       --dim device (-d short option)\n");
	printf("             Dims device. 'device' could either be an integer of the device-id,\n");
	printf("             or the name of the device.\n");
	printf("             Both device-id and name is outputed with the --list option\n");
	printf("             Note: The dimlevel parameter must be set before using this option.\n");
	printf("\n");
	printf("       --dimlevel level (-v short option)\n");
	printf("             Set dim level. 'level' should an integer, 0-255.\n");
	printf("             Note: This parameter must be set before using dim.\n");
	printf("\n");
	printf("       --bell device (-b short option)\n");
	printf("             Sends bell command to devices supporting this. 'device' could\n");
	printf("             either be an integer of the device-id, or the name of the device.\n");
	printf("             Both device-id and name is outputed with the --list option\n");
	printf("\n");
	printf("       --learn device (-e short option)\n");
	printf("             Sends a special learn command to devices supporting this. This is normaly\n");
	printf("             devices of 'selflearning' type. 'device' could either be an integer\n");
	printf("             of the device-id, or the name of the device.\n");
	printf("             Both device-id and name is outputed with the --list option\n");
	printf("\n");
	printf("       --raw input (-r short option)\n");
	printf("             This command sends a raw command to TellStick.\n");
	printf("             input can be either - or a filename. If input is - the data is\n");
	printf("             taken from stdin, otherwise the data is taken from the supplied filename.\n");
	printf("\n");
	printf("             Example to turn on an ArcTech codeswitch A1:\n");
	printf("             echo 'S$k$k$k$k$k$k$k$k$k$k$k$k$k$k$k$k$k$k$kk$$kk$$kk$$}+' | tdtool --raw -\n");
	printf("\n");
	printf("Report bugs to <info.tech@telldus.se>\n");
}

void print_version() {
	printf("tdtool " VERSION "\n");
	printf("\n");
	printf("Copyright (C) 2009 Telldus Technologies AB\n");
	printf("\n");
	printf("Written by Micke Prag <micke.prag@telldus.se>\n");
}

void print_device( int index ) {
	int intId = tdGetDeviceId(index);
	char *name = tdGetName(intId);
	printf("%i\t%s\t", intId, name);
	tdReleaseString(name);
	int lastSentCommand = tdLastSentCommand(intId, SUPPORTED_METHODS);
	char *level = 0;
	switch(lastSentCommand) {
		case TELLSTICK_TURNON:
			printf("ON");
			break;
		case TELLSTICK_TURNOFF:
			printf("OFF");
			break;
		case TELLSTICK_DIM:
			level = tdLastSentValue(intId);
			printf("DIMMED:%s", level);
			tdReleaseString(level);
			break;
		default:
			printf("Unknown state");
	}
	printf("\n");
}

void list_devices() {
	int intNum = tdGetNumberOfDevices();
	printf("Number of devices: %i\n", intNum);
	int i = 0;
	while (i < intNum) {
		print_device( i );
		i++;
	}
}

int find_device( char *device ) {
	int deviceId = atoi(device);
	if (deviceId == 0) { //Try to find the id from the name
		int intNum = tdGetNumberOfDevices();
		int index = 0;
		while (index < intNum) {
			int id = tdGetDeviceId(index);
			char *name = tdGetName( id );
			if (strcasecmp(name, device) == 0) {
				deviceId = id;
 				tdReleaseString(name);
				break;
			}
			tdReleaseString(name);
			index++;
		}
	}
	return deviceId;
}

void switch_device( bool turnOn, char *device ) {
	int deviceId = find_device( device );
	if (deviceId == 0) {
		printf("Device '%s', not found!\n", device);
		return;
	}

	char *name = tdGetName( deviceId );
	int deviceType = tdGetDeviceType( deviceId );
	printf("Turning %s %s %i, %s",
					(turnOn ? "on" : "off"),
					(deviceType == TELLSTICK_TYPE_DEVICE ? "device" : "group"),
					deviceId,
					name);
	tdReleaseString(name);

	int retval = (turnOn ? tdTurnOn( deviceId ) : tdTurnOff( deviceId ));
	char *errorString = tdGetErrorString(retval);
	
	printf(" - %s\n", errorString);
	tdReleaseString(errorString);
}

void dim_device( char *device, int level ) {
	int deviceId = find_device( device );
	if (deviceId == 0) {
		printf("Device '%s', not found!\n", device);
		return;
	}
	if (level < 0 || level > 255) {
		printf("Level %i out of range!\n", level);
		return;
	}

	char *name = tdGetName( deviceId );
	int retval = tdDim( deviceId, (unsigned char)level );
	char *errorString = tdGetErrorString(retval);
	printf("Dimming device: %i %s to %i - %s\n", deviceId, name, level, errorString);
	tdReleaseString(name);
	tdReleaseString(errorString);
}

void bell_device( char *device ) {
	int deviceId = find_device( device );
	if (deviceId == 0) {
		printf("Device '%s', not found!\n", device);
		return;
	}

	char *name = tdGetName( deviceId );
	int retval = tdBell( deviceId );
	char *errorString = tdGetErrorString(retval);
	printf("Sending bell to: %i %s - %s\n", deviceId, name, errorString);
	tdReleaseString(name);
	tdReleaseString(errorString);
}

void learn_device( char *device ) {
	int deviceId = find_device( device );
	if (deviceId == 0) {
		printf("Device '%s', not found!\n", device);
		return;
	}

	char *name = tdGetName( deviceId );
	int retval = tdLearn( deviceId );
	char *errorString = tdGetErrorString(retval);
	printf("Learning device: %i %s - %s\n", deviceId, name, errorString);
	tdReleaseString(name);
	tdReleaseString(errorString);
}

void send_raw_command( char *command ) {
	const int MAX_LENGTH = 100;
	char msg[MAX_LENGTH];
	
	if (strcmp(command, "-") == 0) {
		fgets(msg, MAX_LENGTH, stdin);
	} else {
		FILE *fd;
		
		fd = fopen(command, "r");
		if (fd == NULL) {
			printf("Error opening file %s\n", command);
			return;
		}
		fgets(msg, MAX_LENGTH, fd);
	}
	
	int retval = tdSendRawCommand( msg, 0 );	
	char *errorString = tdGetErrorString(retval);
	printf("Sending raw command: %s\n", errorString);
	tdReleaseString(errorString);
}

int main(int argc, char **argv)
{
	int optch, longindex;
	static char optstring[] = "ln:f:d:b:v:e:r:hi";
	static struct option long_opts[] = {
		{ "list", 0, 0, 'l' },
		{ "on", 1, 0, 'n' },
		{ "off", 1, 0, 'f' },
		{ "dim", 1, 0, 'd' },
		{ "bell", 1, 0, 'b' },
		{ "dimlevel", 1, 0, 'v' },
		{ "learn", 1, 0, 'e' },
		{ "raw", 1, 0, 'r' },
		{ "help", 0, 0, 'h' },
		{ "version", 0, 0, 'i'},
		{ 0, 0, 0, 0}
	};
	int level = -1;

	if (argc < 2) {
		print_usage( argv[0] );
		return -1;
	}

	while ( (optch = getopt_long(argc,argv,optstring,long_opts,&longindex)) != -1 )
		switch (optch) {
			case 'b' :
				bell_device( &optarg[0] );
				break;
			case 'd' :
				if (level >= 0) {
					dim_device( &optarg[0], level );
				}
				break;
			case 'f' :
				switch_device(false, &optarg[0]);
				break;
			case 'h' :
				print_usage( argv[0] );
				break;
			case 'i' :
				print_version( );
				break;
			case 'l' :
				list_devices();
				break;
			case 'n' :
				switch_device(true, &optarg[0]);
				break;
			case 'e' :
				learn_device(&optarg[0]);
				break;
			case 'r' :
				send_raw_command(&optarg[0]);
				break;
			case 'v' :
				level = atoi( &optarg[0] );
				break;
			default :
				print_usage( argv[0] );
				return -1;
		}

	tdClose(); //Cleaning up
	return 0;
}
