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

#include <unistd.h>

std::string TellStick::convertSToT( unsigned char t0, unsigned char t1, unsigned char t2, unsigned char t3, const std::string &data ) {
	unsigned char dataByte = 0;
	std::string retString = "T";
	retString.append(1, t0);
	retString.append(1, t1);
	retString.append(1, t2);
	retString.append(1, t3);

	if (data.length() > 255) {
		return "";
	}
	unsigned char length = (unsigned char)data.length();
	retString.append(1, length);

	for (size_t i = 0; i < data.length(); ++i) {
		dataByte <<= 2;
		if (data.at(i) == 1) {
			dataByte |= 1;
		} else if (data.at(i) == 2) {
			dataByte |= 2;
		} else if (data.at(i) == 3) {
			dataByte |= 3;
		}
		if ( (i+1) % 4 == 0) {
			retString.append(1, dataByte);
			dataByte = 0;
		}
	}
	if (data.length() % 4 != 0) {
		dataByte <<= (data.length() % 4)*2;
		retString.append(1, dataByte);
	}

	retString.append("+");
	return retString;
}
