#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include "TellUsbD101.h"
#ifdef __MINGW32__
  #define sleep(x) _sleep((x)*1000)
#endif

void print_usage( char *name ) {
	printf("Usage:\n");
	printf("   %s [ options ]\n", name);
	printf("\n");
	printf("Options: -[lh] [--list] [--help]\n");
	printf("               [--on device] [--off device]\n");
	printf("\n");
	printf("Options:\n");
	printf("       --list (-l short option)\n");
	printf("             List currently configured devices.\n");
	printf("\n");
	printf("       --help (-h short option)\n");
	printf("             Shows this screen.\n");
	printf("\n");
	printf("       --on device\n");
	printf("             Turns on device. device should be an integer of the device-id.\n");
	printf("             The device-id is outputed with the --list option\n");
	printf("\n");
	printf("       --off device\n");
	printf("             Turns off device. device should be an integer of the device-id.\n");
	printf("             The device-id is outputed with the --list option\n");
	printf("\n");
}

void print_device( int index ) {
	int intId = devGetDeviceId(index);
	char *name = devGetName(intId);
	printf("%i\t%s\n", intId, name);
}

void list_devices() {
	int intNum = devGetNumberOfDevices();
	printf("Number of devices: %i\n", intNum);
	int i = 0;
	while (i < intNum) {
		print_device( i );
		i++;
	}
}

void switch_device( bool turnOn, int device ) {
	if (turnOn) {
		char *name = devGetName( device );
		bool ok = devTurnOn( device );
		printf("Turning on device: %i %s - %s\n", device, name, (ok ? "ok" : "failed"));
	} else {
		char *name = devGetName( device );
		bool ok = devTurnOff( device );
		printf("Turning off device: %i %s - %s\n", device, name, (ok ? "ok" : "failed"));
	}
	sleep(1);
}

int main(int argc, char **argv)
{
	int optch, longindex;
	static char optstring[] = "ln:f:h";
	static struct option long_opts[] = {
		{ "list", 0, 0, 'l' },
		{ "on", 1, 0, 'n' },
		{ "off", 1, 0, 'f' },
		{ "help", 1, 0, 'h' },
		{ 0, 0, 0, 0}
	};

	while ( (optch = getopt_long(argc,argv,optstring,long_opts,&longindex)) != -1 )
		switch (optch) {
			case 'f' :
				switch_device(false, atoi(&optarg[0]));
				break;
			case 'h' :
				print_usage( argv[0] );
				break;
			case 'l' :
				list_devices();
				break;
			case 'n' :
				switch_device(true, atoi(&optarg[0]));
				break;
			default :
				print_usage( argv[0] );
				return -1;
		}

	return 0;
}
