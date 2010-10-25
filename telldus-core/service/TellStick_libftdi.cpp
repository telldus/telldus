//
// C++ Implementation: TellStick
//
// Description:
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "TellStick.h"
#include "../client/telldus-core.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <ftdi.h>

class TellStick::PrivateData {
public:
	bool open;
	int vid, pid, fwVersion;
	std::string serial, message;
	ftdi_context ftHandle;
};

TellStick::TellStick(int controllerId, Event *event, const TellStickDescriptor &td )
	:Controller(controllerId, event)
{
	d = new PrivateData;
	d->open = false;
	d->vid = td.vid;
	d->pid = td.pid;
	d->fwVersion = 0;
	d->serial = td.serial;

	ftdi_init(&d->ftHandle);

	int ret = ftdi_usb_open_desc(&d->ftHandle, td.vid, td.pid, NULL, td.serial.c_str());
	if (ret < 0) {
		ftdi_deinit(&d->ftHandle);
		return;
	}
	d->open = true;
	ftdi_usb_reset( &d->ftHandle );
	ftdi_disable_bitbang( &d->ftHandle );

	if (d->open) {
// 		setBaud(4800);
	}
}

TellStick::~TellStick() {
	if (d->open) {
		ftdi_usb_close(&d->ftHandle);
		ftdi_deinit(&d->ftHandle);
	}
	delete d;
}

int TellStick::firmwareVersion() {
	return 1;
}

bool TellStick::isOpen() const {
	return d->open;
}

bool TellStick::isSameAsDescriptor(const TellStickDescriptor &td) const {
	if (td.vid != d->vid) {
		return false;
	}
	if (td.pid != d->pid) {
		return false;
	}
	if (td.serial != d->serial) {
		return false;
	}
	return true;
}

void TellStick::run() {
	//TODO
}

int TellStick::send( const std::string &message ) {
	return TELLSTICK_SUCCESS;
}

std::list<TellStickDescriptor> TellStick::findAll() {
	std::list<TellStickDescriptor> tellstick = findAllByVIDPID(0x1781, 0x0C30);

	std::list<TellStickDescriptor> duo = findAllByVIDPID(0x1781, 0x0C31);
	for(std::list<TellStickDescriptor>::const_iterator it = duo.begin(); it != duo.end(); ++it) {
		tellstick.push_back(*it);
	}

	return tellstick;

}

bool TellStick::stillConnected() const {
	ftdi_context ftdic;
	struct ftdi_device_list *devlist, *curdev;
	char serialBuffer[10];
	ftdi_init(&ftdic);
	bool found = false;

	int ret = ftdi_usb_find_all(&ftdic, &devlist, d->vid, d->pid);
	if (ret > 0) {
		for (curdev = devlist; curdev != NULL; curdev = curdev->next) {
			ret = ftdi_usb_get_strings(&ftdic, curdev->dev, NULL, 0, NULL, 0, serialBuffer, 10);
			if (ret != 0) {
				continue;
			}
			if (d->serial.compare(serialBuffer) == 0) {
				found = true;
				break;
			}
		}
	}

	ftdi_list_free(&devlist);
	ftdi_deinit(&ftdic);
	return found;
}

std::list<TellStickDescriptor> TellStick::findAllByVIDPID( int vid, int pid ) {
	std::list<TellStickDescriptor> retval;

	ftdi_context ftdic;
	struct ftdi_device_list *devlist, *curdev;
	char serialBuffer[10];
	ftdi_init(&ftdic);

	int ret = ftdi_usb_find_all(&ftdic, &devlist, vid, pid);
	if (ret > 0) {
		for (curdev = devlist; curdev != NULL; curdev = curdev->next) {
			ret = ftdi_usb_get_strings(&ftdic, curdev->dev, NULL, 0, NULL, 0, serialBuffer, 10);
			if (ret != 0) {
				continue;
			}
			TellStickDescriptor td;
			td.vid = vid;
			td.pid = pid;
			td.serial = serialBuffer;
			retval.push_back(td);
		}
	}
	ftdi_list_free(&devlist);
	ftdi_deinit(&ftdic);

	return retval;
}
