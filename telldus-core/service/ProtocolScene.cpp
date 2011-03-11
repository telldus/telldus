#include "ProtocolScene.h"

int ProtocolScene::methods() const {
	return TELLSTICK_EXECUTE;
}

std::string ProtocolScene::getStringForMethod(int method, unsigned char data, Controller *) {
	return "";
}
