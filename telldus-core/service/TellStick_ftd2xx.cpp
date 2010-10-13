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

#include "ftd2xx.h"

class TellStick::PrivateData {
public:
	bool open;
	FT_HANDLE ftHandle;
};

TellStick::TellStick( const TellStickDescriptor &td ) {
	d = new PrivateData;
	d->open = false;
	
	if (d->open) {
// 		setBaud(4800);
	}
}

TellStick::~TellStick() {
	if (d->open) {
	}
	delete d;
}

int TellStick::firmwareVersion() {
	return 1;
}

bool TellStick::isOpen() const {
	return d->open;
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

std::list<TellStickDescriptor> TellStick::findAllByVIDPID( int vid, int pid ) {
	std::list<TellStickDescriptor> retval;
	

	return retval;
}
