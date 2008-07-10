#include "../Device.h"
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

/*
* Send message to the USB dongle
*/
void Device::send(char* strMessage) {
	int fd = -1;
	struct termios tio;

	if( 0 > ( fd = open( strDevice, O_RDWR ) ) ) {
		return;
	}

	/* adjust serial port parameters */
	bzero(&tio, sizeof(tio)); /* clear struct for new port settings */
	tio.c_cflag = B4800 | CS8 | CLOCAL | CREAD; /* CREAD not used yet */
	tio.c_iflag = IGNPAR;
	tio.c_oflag = 0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&tio);

	write(fd, strMessage, strlen(strMessage));

	close(fd);
}

void Device::setDevice(const char *device) {
	strDevice = strdup(device);
}
