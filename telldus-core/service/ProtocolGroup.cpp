#include "ProtocolGroup.h"

int ProtocolGroup::methods() const {
	return 0;
}

std::string ProtocolGroup::getStringForMethod(int method, unsigned char data, Controller *) {
	return "";
}