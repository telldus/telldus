#include "ProtocolWaveman.h"

std::string ProtocolWaveman::getStringForMethod(int method, unsigned char, Controller *) {
	return getStringCodeSwitch(method);
}

std::string ProtocolWaveman::getOffCode() const {
	return "$k$k$k$k$k$k$k$k$k+";
}
