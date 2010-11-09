#include "ProtocolComen.h"

int ProtocolComen::methods() const {
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}

std::string ProtocolComen::getStringForMethod(int method, unsigned char level, Controller *) {
	int intHouse = getIntParameter(L"house", 1, 33554431);
	intHouse <<= 1; //They seem to only accept even codes?
	int intCode = getIntParameter(L"unit", 1, 16)-1;
	return getStringSelflearningForCode(intHouse, intCode, method, level);
}
