#include "../Device.h"
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

using namespace TelldusCore;

/*
* Send message to the USB dongle
*/
int Device::send(const std::string &strMessage) {
	int fd = -1;
	struct termios tio;

	if( 0 > ( fd = open( strDevice.c_str(), O_RDWR ) ) ) {
		if (errno == ENOENT) {
			return TELLSTICK_ERROR_NOT_FOUND;
		} else if (errno == EACCES) {
			return TELLSTICK_ERROR_PERMISSION_DENIED;
		}
		return TELLSTICK_ERROR_UNKNOWN;
	}

	/* adjust serial port parameters */
	bzero(&tio, sizeof(tio)); /* clear struct for new port settings */
	tio.c_cflag = B4800 | CS8 | CLOCAL | CREAD; /* CREAD not used yet */
	tio.c_iflag = IGNPAR;
	tio.c_oflag = 0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&tio);

 	write(fd, strMessage.c_str(), strMessage.length());

	close(fd);
	
	sleep(1);
	
	return TELLSTICK_SUCCESS;
}

void Device::setDevice(const std::string &device) {
	strDevice = device;
}
