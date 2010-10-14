#include "ProtocolIkea.h"

#ifdef _WINDOWS
#define strcasecmp(x, y) _strcmpi(x, y)
#endif

/*
bool DeviceIkea::setSystem(const std::string &strSystem) {
	if (strSystem.length() > 0) {
		intSystem = atoi(strSystem.c_str()) - 1;
	} else {
		intSystem = 0;
	}
	return true;
}

bool DeviceIkea::setUnits(const std::string &strUnits) {
	if (strUnits.length() > 0) {
		intUnits = 0; //Start without any units

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
	}
	return true;
}

bool DeviceIkea::setFade(const std::string &strFadeStyle) {
	if (strFadeStyle.length() > 0 && strcasecmp(strFadeStyle.c_str(), "true") == 0) {
		intFadeStyle = 1;
	} else {
		intFadeStyle = 0;
	}
	return true;
}


bool DeviceIkea::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("system") == 0) {
		return setSystem(strValue);
	} else if (strName.compare("units") == 0) {
		return setUnits(strValue);
	} else if (strName.compare("fade") == 0) {
		return setFade(strValue);
	}
	return false;
}


int DeviceIkea::turnOn(Controller *controller){
	try{
		std::string strMessage = getStringCode(255);

		return controller->send(strMessage);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}


int DeviceIkea::turnOff(Controller *controller){
	try{
		std::string strMessage = getStringCode(0);

		return controller->send(strMessage);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}


int DeviceIkea::dim(unsigned char level, Controller *controller){
	try{
		std::string strMessage = getStringCode(level);

		return controller->send(strMessage);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}*/

std::string ProtocolIkea::getStringForMethod(int method, const std::string &strLevel, Controller *) {
	int intSystem = this->getIntParameter(L"system", 1, 16)-1;
	int intFadeStyle = comparei(this->getStringParameter(L"fade", L"true"), L"true");
	std::wstring wstrUnits = this->getStringParameter(L"units", L"");

	unsigned char level;
	if (method == TELLSTICK_TURNON) {
		level = 255;
	} else if (method == TELLSTICK_TURNOFF) {
		level = 0;
	} else if (method == TELLSTICK_DIM) {
		level = 50; //TODO
	} else {
		return "";
	}

	if (wstrUnits == L"") {
		return "";
	}

	std::string strUnits(wstrUnits.begin(), wstrUnits.end());
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
