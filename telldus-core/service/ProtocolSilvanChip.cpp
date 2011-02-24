#include "ProtocolSilvanChip.h"
#include "Strings.h"

int ProtocolSilvanChip::methods() const {
	if (TelldusCore::comparei(model(), L"kp100")) {
		return TELLSTICK_UP | TELLSTICK_DOWN | TELLSTICK_STOP | TELLSTICK_LEARN;
	}
	return TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_LEARN;
}

std::string ProtocolSilvanChip::getStringForMethod(int method, unsigned char data, Controller *controller) {
	if (TelldusCore::comparei(model(), L"kp100")) {
		std::string preamble;
		preamble.append(1, 100);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 255);
		preamble.append(1, 1);
		preamble.append(1, 100);

		const std::string one = "\xFF\x1\x2E\x2E";
		const std::string zero = "\x2E\xFF\x1\x2E";
		int button = 0;
		if (method == TELLSTICK_UP) {
			button = 2;
		} else if (method == TELLSTICK_DOWN) {
			button = 4;
		} else if (method == TELLSTICK_STOP) {
			button = 3;
		} else if (method == TELLSTICK_LEARN) {
			button = 1;
		} else {
			return "";
		}
		return this->getString(preamble, one, zero, button);
	}
	return "";
}

std::string ProtocolSilvanChip::getString(const std::string &preamble, const std::string &one, const std::string &zero, int button) {

	int intHouse = this->getIntParameter(L"house", 1, 1048575);
	std::string strReturn = preamble;

	for( int i = 19; i >= 0; --i ) {
		if (intHouse & (1 << i)) {
			strReturn.append(one);
		} else {
			strReturn.append(zero);
		}
	}

	if (button == 1) {
		strReturn.append(zero);
		strReturn.append(zero);
		strReturn.append(zero);
		strReturn.append(one);
	} else if (button == 2) {
		strReturn.append(zero);
		strReturn.append(zero);
		strReturn.append(one);
		strReturn.append(zero);
	} else if (button == 3) {
		strReturn.append(zero);
		strReturn.append(one);
		strReturn.append(zero);
		strReturn.append(zero);
	} else if (button == 4) {
		strReturn.append(one);
		strReturn.append(zero);
		strReturn.append(zero);
		strReturn.append(zero);
	} else {
		return "";
	}

	strReturn.append(zero);
	return strReturn;
}
