#include "ProtocolIkea.h"
#include "Strings.h"

#include <stdlib.h>
#include <string.h>

int ProtocolIkea::methods() const {
	return TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_DIM;
}

std::string ProtocolIkea::getStringForMethod(int method, unsigned char level, Controller *) {
	int intSystem = this->getIntParameter(L"system", 1, 16)-1;
	int intFadeStyle = TelldusCore::comparei(this->getStringParameter(L"fade", L"true"), L"true");
	std::wstring wstrUnits = this->getStringParameter(L"units", L"");

	if (method == TELLSTICK_TURNON) {
		level = 255;
	} else if (method == TELLSTICK_TURNOFF) {
		level = 0;
	} else if (method == TELLSTICK_DIM) {
	} else {
		return "";
	}

	if (wstrUnits == L"") {
		return "";
	}

	std::string strUnits(TelldusCore::wideToString(wstrUnits));
	int intUnits = 0; //Start without any units

	char *tempUnits = new char[strUnits.size()+1];
#ifdef _WINDOWS
	strcpy_s(tempUnits, strUnits.size()+1, strUnits.c_str());
#else
	strcpy(tempUnits, strUnits.c_str());
#endif

	char *strToken = strtok(tempUnits, ",");
	do {
		int intUnit = atoi(strToken);
		if (intUnit == 10) {
			intUnit = 0;
		}
		intUnits = intUnits | ( 1<<(9-intUnit) );
	} while ( (strToken = strtok(NULL, ",")) != NULL );

	free(tempUnits);

	std::string strReturn = "STTTTTTª"; //Startcode, always like this;

	std::string strChannels = "";
	int intCode = (intSystem << 10) | intUnits;
	int checksum1 = 0;
	int checksum2 = 0;
	for (int i = 13; i >= 0; --i) {
		if ((intCode>>i) & 1) {
			strChannels.append("TT");
			if (i % 2 == 0)
				checksum2++;
			else
				checksum1++;
		} else {
			strChannels.append("ª");
		}
	}
	strReturn.append(strChannels); //System + Units

	strReturn.append(checksum1 %2 == 0 ? "TT" : "ª"); //1st checksum
	strReturn.append(checksum2 %2 == 0 ? "TT" : "ª"); //2nd checksum

	int intLevel = 0;
	if (level <= 12) {
		intLevel = 10; // Level 10 is actually off
	} else if (level <= 37) {
		intLevel = 1;
	} else if (level <= 62) {
		intLevel = 2;
	} else if (level <= 87) {
		intLevel = 3;
	} else if (level <= 112) {
		intLevel = 4;
	} else if (level <= 137) {
		intLevel = 5;
	} else if (level <= 162) {
		intLevel = 6;
	} else if (level <= 187) {
		intLevel = 7;
	} else if (level <= 212) {
		intLevel = 8;
	} else if (level <= 237) {
		intLevel = 9;
	} else {
		intLevel = 0; // Level 0 is actually full on
	}

	int intFade = 0;
	if (intFadeStyle == 1) {
		intFade = 11 << 4; //Smooth
	} else {
		intFade = 1 << 4; //Instant
	}

	intCode = intLevel | intFade; //Concat level and fade

	checksum1 = 0;
	checksum2 = 0;
	for (int i = 0; i < 6; ++i) {
		if ((intCode>>i) & 1) {
			strReturn.append("TT");
			if (i % 2 == 0)
				checksum1++;
			else
				checksum2++;
		} else {
			strReturn.append("ª");
		}
	}

	strReturn.append(checksum1 %2 == 0 ? "TT" : "ª"); //1st checksum
	strReturn.append(checksum2 %2 == 0 ? "TT" : "ª"); //2nd checksum

	strReturn.append("+");

	return strReturn;

}
