#include "ProtocolYidong.h"

std::string ProtocolYidong::getStringForMethod(int method, unsigned char, Controller *) {
	int intCode = this->getIntParameter(L"unit", 1, 4);
	std::wstring strCode = L"111";

	switch(intCode) {
	case 1:
		strCode.append(L"0010");
		break;
	case 2:
		strCode.append(L"0001");
		break;
	case 3:
		strCode.append(L"0100");
		break;
	case 4:
		strCode.append(L"1000");
		break;
	}

	strCode.append(L"110");
	return getStringForCode(strCode, method);
}
