#include "ProtocolX10.h"

int ProtocolX10::methods() const {
	return TELLSTICK_TURNON | TELLSTICK_TURNOFF;
}

std::string ProtocolX10::getStringForMethod(int method, unsigned char data, Controller *controller) {
	const unsigned char S = 59, L = 169;
	const char B0[] = {S,S,0};
	const char B1[] = {S,L,0};
	const int HOUSES[] = {6,0xE,2,0xA,1,9,5,0xD,7,0xF,3,0xB,0,8,4,0xC};
	const unsigned char START_CODE[] = {'S',255,1,255,1,255,1,100,255,1,180,0};
	const unsigned char STOP_CODE[] = {S,0};

	std::string strReturn = reinterpret_cast<const char*>(START_CODE);
	std::string strComplement = "";

	std::wstring strHouse = getStringParameter(L"house", L"A");
	int intHouse = strHouse[0] - L'A';
	if (intHouse < 0) {
		intHouse = 0;
	} else if (intHouse > 15) {
		intHouse = 15;
	}
	//Translate it
	intHouse = HOUSES[intHouse];
	int intCode = getIntParameter(L"unit", 1, 16)-1;

	for( int i = 0; i < 4; ++i ) {
		if (intHouse & 1) {
			strReturn.append(B1);
			strComplement.append(B0);
		} else {
			strReturn.append(B0);
			strComplement.append(B1);
		}
		intHouse >>= 1;
	}
	strReturn.append( B0 );
	strComplement.append( B1 );

	if (intCode >= 8) {
		strReturn.append(B1);
		strComplement.append(B0);
	} else {
		strReturn.append(B0);
		strComplement.append(B1);
	}

	strReturn.append( B0 );
	strComplement.append( B1 );
	strReturn.append( B0 );
	strComplement.append( B1 );

	strReturn.append( strComplement );
	strComplement = "";

	strReturn.append( B0 );
	strComplement.append( B1 );

	if (intCode >> 2 & 1) { //Bit 2 of intCode
		strReturn.append(B1);
		strComplement.append(B0);
	} else {
		strReturn.append(B0);
		strComplement.append(B1);
	}

	if (method == TELLSTICK_TURNON) {
		strReturn.append(B0);
		strComplement.append(B1);
	} else if (method == TELLSTICK_TURNOFF) {
		strReturn.append(B1);
		strComplement.append(B0);
	} else {
		return "";
	}

	if (intCode & 1) { //Bit 0 of intCode
		strReturn.append(B1);
		strComplement.append(B0);
	} else {
		strReturn.append(B0);
		strComplement.append(B1);
	}

	if (intCode >> 1 & 1) { //Bit 1 of intCode
		strReturn.append(B1);
		strComplement.append(B0);
	} else {
		strReturn.append(B0);
		strComplement.append(B1);
	}

	for( int i = 0; i < 3; ++i ) {
		strReturn.append( B0 );
		strComplement.append( B1 );
	}

	strReturn.append( strComplement );
	strReturn.append( reinterpret_cast<const char*>(STOP_CODE) );
	strReturn.append("+");
	return strReturn;

}
