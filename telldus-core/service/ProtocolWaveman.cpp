#include "ProtocolWaveman.h"

int ProtocolWaveman::methods() const {
	return TELLSTICK_TURNON | TELLSTICK_TURNOFF;
}

std::string ProtocolWaveman::getStringForMethod(int method, unsigned char, Controller *) {
	return getStringCodeSwitch(method);
}

std::string ProtocolWaveman::getOffCode() const {
	return "$k$k$k$k$k$k$k$k$k+";
}
