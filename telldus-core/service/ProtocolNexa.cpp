#include "ProtocolNexa.h"

std::string ProtocolNexa::getStringForMethod(int method, unsigned char data, Controller *) {
	if (!comparei(model(), L"codeswitch")) {
		return getStringSelflearning(method, data);
	}
	return getStringCodeSwitch(method);
}

std::string ProtocolNexa::getStringCodeSwitch(int method) {
	std::string strReturn = "S";

	std::wstring house = getStringParameter(L"house", L"A");
	int intHouse = house[0] - L'A';
	strReturn.append(getCodeSwitchTuple(intHouse));
	strReturn.append(getCodeSwitchTuple(getIntParameter(L"unit", 1, 16)-1));

	if (method == TELLSTICK_TURNON) {
		strReturn.append("$k$k$kk$$kk$$kk$$k+");
	} else if (method == TELLSTICK_TURNOFF) {
		strReturn.append(this->getOffCode());
	} else {
		return "";
	}
	return strReturn;
}

std::string ProtocolNexa::getStringSelflearning(int method, unsigned char level) {
	const char START[] = {'T',127,255,24,1,0};
//	const char START[] = {'T',130,255,26,24,0};

	std::string strMessage(START);
	strMessage.append(1,(method == TELLSTICK_DIM ? 147 : 132)); //Number of pulses
	int intHouse = getIntParameter(L"house", 1, 67108863);
	int intCode = getIntParameter(L"unit", 1, 16)-1;
		
	std::string m;
	for (int i = 25; i >= 0; --i) {
		m.append( intHouse & 1 << i ? "10" : "01" );
	}
	m.append("01"); //Group
	
	//On/off
	if (method == TELLSTICK_DIM) {
		m.append("00");
	} else if (method == TELLSTICK_TURNOFF) {
		m.append("01");
	} else if (method == TELLSTICK_TURNON) {
		m.append("10");
	} else {
		return "";
	}
	
	for (int i = 3; i >= 0; --i) {
		m.append( intCode & 1 << i ? "10" : "01" );
	}
	
	if (method == TELLSTICK_DIM) {
		unsigned char newLevel = level/16;
		for (int i = 3; i >= 0; --i) {
			m.append(newLevel & 1 << i ? "10" : "01");
		}
	}
	
	//The number of data is odd.
	//Add this to make it even, otherwise the following loop will not work
	m.append("0"); 
	
	unsigned char code = 9; //b1001, startcode
	for (unsigned int i = 0; i < m.length(); ++i) {
		code <<= 4;
		if (m[i] == '1') {
			code |= 8; //b1000
		} else {
			code |= 10; //b1010
//			code |= 11; //b1011
		}
		if (i % 2 == 0) {
			strMessage.append(1,code);
			code = 0;
		}
	}
	strMessage.append("+");
	
// 	for( int i = 0; i < strMessage.length(); ++i ) {
// 		printf("%i,", (unsigned char)strMessage[i]);
// 	}
// 	printf("\n");
	return strMessage;
}

std::string ProtocolNexa::getCodeSwitchTuple(int intCode) {
	std::string strReturn = "";
	for( int i = 0; i < 4; ++i ) {
		if (intCode & 1) { //Convert 1
			strReturn.append("$kk$");
		} else { //Convert 0
			strReturn.append("$k$k");
		}
		intCode >>= 1;
	}
	return strReturn;
}

std::string ProtocolNexa::getOffCode() const {
	return "$k$k$kk$$kk$$k$k$k+";
}
