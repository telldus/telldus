/*
 * TellStick libftdi libusb version
 * tested with 0.15 : http://www.intra2net.com/en/developer/libftdi
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftdi.h>

#define BAUD 4800

void ftdiCleanup(struct ftdi_context *ctx) {
	ftdi_usb_close( ctx );
	ftdi_deinit( ctx );
}

/*
 * use libftdi and libusb to send command
 * no kernel driver needed
 */
int usbWriteFtdi(char *cmdstr)
{
	struct ftdi_context ctx;
	int device=0x0c30, vendor=0x1781;
	int retrycnt;
	int retval = 0;

	if (ftdi_init( &ctx )) {
		char *err = ftdi_get_error_string(&ctx);
		fprintf(stderr,  "usb - init error: %s\n", err);
		return 1;
	}

	retval = ftdi_usb_open(&ctx, vendor, device);
	if (retval) {
		char *err = ftdi_get_error_string(&ctx);
		// FreeBSD says -3 when another rfcmd is running...
		// Same on other systems?
		if(retval == -3) {
			fprintf(stderr,  "usb - open error: %s. Is it busy?\n", err);
		} else {
			fprintf(stderr,  "usb - open error: %s\n", err);
		}

		ftdi_deinit( &ctx );
		return 2;
	}

	if (ftdi_usb_reset( &ctx )) {
		char *err = ftdi_get_error_string(&ctx);
		fprintf(stderr,  "usb - reset error: %s\n", err);
		retval = 3;
		ftdiCleanup(&ctx);
		return retval;
	}

	if (ftdi_disable_bitbang( &ctx ) || ftdi_set_baudrate(&ctx, BAUD)) {
		char *err = ftdi_get_error_string(&ctx);
		fprintf(stderr,  "usb - init failed: %s\n", err);
		ftdiCleanup(&ctx);
		return 4;
	}

	retval = ftdi_write_data( &ctx, cmdstr, strlen(cmdstr) );
	if (retval < 0) {
		char *err = ftdi_get_error_string(&ctx);
		fprintf(stderr,  "usb - write failed: %s\n", err);
		ftdiCleanup(&ctx);
		return 5;
	}	else if(retval != strlen(cmdstr)) {
		fprintf(stderr,  "usb - warning: %d bytes written instead of %d\n", 
				retval, (int)strlen(cmdstr));
	}

	/**
	 * Wait for Tellstick to be done with cmd, read back until we've received
	 * a \n indicating end of response.
	 * Wait max 5000 * 1000uS.
	 * XXX: Can the tellstick report errors?
	 */
	retval = 0;
	retrycnt = 5000;
	while (retrycnt--) {
		unsigned char inb;
		int bytes;

		bytes = ftdi_read_data(&ctx, &inb, 1);
		if (bytes == 0) {
			usleep(1000);
		} else if (bytes > 0) {
			// Done when newline is received
			if(inb == '\n') {
				ftdiCleanup(&ctx);
				return retval;
			}
		} else {
			char *err = ftdi_get_error_string(&ctx);
			fprintf(stderr,  "usb - read error: %s\n", err);
			ftdiCleanup(&ctx);
			return 6;
		}
	}

	// if we get here we failed to readback
	fprintf(stderr,  "usb - warning: never got newline response, giving up on wait\n");
	return retval;
}


