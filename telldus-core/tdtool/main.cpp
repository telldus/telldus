#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../driver/libtelldus-core/telldus-core.h"
#ifdef __MINGW32__
  #define sleep(x) _sleep((x)*1000)
#endif

void print_usage( char *name ) {
	printf("\"tdtool\" is a command line utility to control a Telldus TellStick\n");
	printf("\n");
	printf("Usage: %s [ options ]\n", name);
	printf("\n");
	printf("Options:\n");
	printf("         -[bdfhlnv] [ --list ] [ --help ]\n");
	printf("                    [ --on device ] [ --off device ] [ --bell device ]\n");
	printf("                    [ --dimlevel level --dim device ]\n");
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
	printf("Written by Micke Prag <micke.prag@telldus.se>\n");
	printf("\n");
	printf("Copyright (C) 2008 Telldus Technologies AB\n");
}

void print_version() {
	printf("tdtool 2.0\n");
	printf("\n");
	printf("Report bugs to <info.tech@telldus.se>\n");
}

void print_device( int index ) {
	int intId = tdGetDeviceId(index);
	char *name = tdGetName(intId);
	printf("%i\t%s\n", intId, name);
	free(name);
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
				free(name);
				break;
			}
			free(name);
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
	int retval = (turnOn ? tdTurnOn( deviceId ) : tdTurnOff( deviceId ));
	char *errorString = tdGetErrorString(retval);
	
	printf("Turning %s device: %i %s - %s\n", (turnOn ? "on" : "off"), deviceId, name, errorString);
	free(name);
	free(errorString);
	if (retval == TELLSTICK_SUCCESS) {
		sleep(1);
	}
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
	free(name);
	free(errorString);
	if (retval == TELLSTICK_SUCCESS) {
		sleep(1);
	}
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
	free(name);
	free(errorString);
	if (retval == TELLSTICK_SUCCESS) {
		sleep(1);
	}
}

int main(int argc, char **argv)
{
	int optch, longindex;
	static char optstring[] = "ln:f:d:b:v:hi";
	static struct option long_opts[] = {
		{ "list", 0, 0, 'l' },
		{ "on", 1, 0, 'n' },
		{ "off", 1, 0, 'f' },
		{ "dim", 1, 0, 'd' },
		{ "bell", 1, 0, 'b' },
		{ "dimlevel", 1, 0, 'v' },
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
			case 'v' :
				level = atoi( &optarg[0] );
				break;
			default :
				print_usage( argv[0] );
				return -1;
		}

	return 0;
}
