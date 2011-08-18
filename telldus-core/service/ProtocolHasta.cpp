#include "ProtocolHasta.h"
#include <sstream>
#include <stdio.h>

int ProtocolHasta::methods() const {
	return TELLSTICK_UP | TELLSTICK_DOWN | TELLSTICK_STOP | TELLSTICK_LEARN;
}

std::string ProtocolHasta::getStringForMethod(int method, unsigned char, Controller *) {
	int house = this->getIntParameter(L"house", 1, 65536);
	int unit = this->getIntParameter(L"unit", 1, 15);
	std::string strReturn;

	std::string preamble;
	strReturn.append(1, 190);
	strReturn.append(1, 1);
	strReturn.append(1, 190);
	strReturn.append(1, 1);
	strReturn.append(1, 190);
	strReturn.append(1, 190);

	strReturn.append(convertByte( (house&0xFF) ));
	strReturn.append(convertByte( (house>>8)&0xFF ));

	int byte = unit&0x0F;

	if (method == TELLSTICK_UP) {
		byte |= 0x00;

	} else if (method == TELLSTICK_DOWN) {
		byte |= 0x10;

	} else if (method == TELLSTICK_STOP) {
		byte |= 0x50;

	} else if (method == TELLSTICK_LEARN) {
		byte |= 0x40;

	} else {
		return "";
	}
	strReturn.append(convertByte(byte));

	strReturn.append(convertByte(0x0));
	strReturn.append(convertByte(0x0));

	//Remove the last pulse
	strReturn.erase(strReturn.end()-1,strReturn.end());

	return strReturn;
}

std::string ProtocolHasta::convertByte(unsigned char byte) {
	std::string retval;
	for(int i = 0; i < 8; ++i) {
		if (byte & 1) {
			retval.append(1, 33);
			retval.append(1, 17);
		} else {
			retval.append(1, 17);
			retval.append(1, 33);
		}
		byte >>= 1;
	}
	return retval;
}