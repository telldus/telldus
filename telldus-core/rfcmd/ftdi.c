/*
 * TellStick libftdi libusb version
 * tested with 0.15 : http://www.intra2net.com/en/developer/libftdi
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftdi.h>

#define BAUD 4800

/*
 * use libftdi and libusb to send command
 * no kernel driver needed
 */
int usbWriteFtdi(char *cmdstr)
{
	struct ftdi_context ctx;
	int device=0x0c30, vendor=0x1781;

	if (ftdi_init( &ctx )) {
		fprintf(stderr,  "usb - init error !\n");
		return 1;
	}

	if (ftdi_usb_open(&ctx, vendor, device)) {
		fprintf(stderr,  "usb - open error (cannot find?) !\n");
		ftdi_deinit( &ctx );
		return 2;
	}

	if (ftdi_usb_reset( &ctx )) {
		fprintf(stderr,  "usb - reset error !\n");
		ftdi_usb_close( &ctx );
		ftdi_deinit( &ctx );
		return 3;
	}

	ftdi_disable_bitbang( &ctx );
	ftdi_set_baudrate(&ctx, BAUD);

	ftdi_write_data( &ctx, cmdstr, strlen(cmdstr) );
	sleep(1); /* just for sure */
	ftdi_usb_close( &ctx );
	ftdi_deinit( &ctx );

	return 0;
}


