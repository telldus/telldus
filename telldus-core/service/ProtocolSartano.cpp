#include "ProtocolSartano.h"

int ProtocolSartano::methods() const {
	return TELLSTICK_TURNON | TELLSTICK_TURNOFF;
}

std::string ProtocolSartano::getStringForMethod(int method, unsigned char, Controller *) {
	std::wstring strCode = this->getStringParameter(L"code", L"");
	return getStringForCode(strCode, method);
}

std::string ProtocolSartano::getStringForCode(const std::wstring &strCode, int method) {

	std::string strReturn("S");

	for (size_t i = 0; i < strCode.length(); ++i) {
		if (strCode[i] == L'1') {
			strReturn.append("$k$k");
		} else {
			strReturn.append("$kk$");
		}
	}

	if (method == TELLSTICK_TURNON) {
		strReturn.append("$k$k$kk$$k+");
	} else if (method == TELLSTICK_TURNOFF) {
		strReturn.append("$kk$$k$k$k+");
	} else {
		return "";
	}

	return strReturn;

}
