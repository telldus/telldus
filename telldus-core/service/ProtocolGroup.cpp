#include "ProtocolGroup.h"

int ProtocolGroup::methods() const {
	return TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM | TELLSTICK_BELL | TELLSTICK_LEARN | TELLSTICK_EXECUTE | TELLSTICK_TOGGLE;
}

std::string ProtocolGroup::getStringForMethod(int method, unsigned char data, Controller *) {
	return "";
}