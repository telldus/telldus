#include "ProtocolNexa.h"

ProtocolNexa::ProtocolNexa()
	:Protocol()
{
	
}

/*
* Destructor
*/
ProtocolNexa::~ProtocolNexa(void)
{}

std::string ProtocolNexa::getStringForMethod(int method, const std::string &data, Controller *) {
//	const char START[] = {'T',127,255,24,1,0};
	const char START[] = {'T',130,255,26,24,0};

	std::string strMessage(START);
	strMessage.append(1,(method == TELLSTICK_DIM ? 147 : 132)); //Number of pulses
	int intHouse = getIntParameter(L"house", 1, 67108863);
	int intCode = getIntParameter(L"code", 1, 16)-1;
		
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
	} else {
		m.append("10");
	}
	
	for (int i = 3; i >= 0; --i) {
		m.append( intCode & 1 << i ? "10" : "01" );
	}
	
	if (method == TELLSTICK_DIM) {
		/*unsigned char newLevel = level/16;
		for (int i = 3; i >= 0; --i) {
			m.append(newLevel & 1 << i ? "10" : "01");
		}*/
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
