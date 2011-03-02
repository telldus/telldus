#include "Strings.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <iconv.h>

std::wstring TelldusCore::charToWstring(const char *value) {
	size_t utf8Length = strlen(value);
	size_t outbytesLeft = utf8Length*sizeof(wchar_t);

	iconv_t convDesc = iconv_open("WCHAR_T", "UTF-8");

	//Copy the instring
	char *inString = new char[strlen(value)+1];
	strcpy(inString, value);

	//Create buffer for output
	char *outString = (char*)new wchar_t[utf8Length+1];
	memset(outString, 0, sizeof(wchar_t)*(utf8Length+1));

	char *inPointer = inString;
	char *outPointer = outString;

	iconv(convDesc, &inPointer, &utf8Length, &outPointer, &outbytesLeft);

	std::wstring retval( (wchar_t *)outString );

	//Cleanup
	iconv_close(convDesc);
	delete[] inString;
	delete[] outString;

	return retval;
}

int TelldusCore::charToInteger(const char *input){
	std::stringstream inputstream;
	inputstream << input;
	int retval;
	inputstream >> retval;
	return retval;
}

/**
* This method doesn't support all locales
*/
bool TelldusCore::comparei(std::wstring stringA, std::wstring stringB) {
	transform(stringA.begin(), stringA.end(), stringA.begin(), toupper);
	transform(stringB.begin(), stringB.end(), stringB.begin(), toupper);

	return stringA == stringB;
}

int TelldusCore::wideToInteger(const std::wstring &input){
	std::wstringstream inputstream;
	inputstream << input;
	int retval;
	inputstream >> retval;
	return retval;
}

std::string TelldusCore::wideToString(const std::wstring &input) {
	std::string strReturn;
	size_t len = input.length();
	char* convPointer = new char[len + 1];
	wcstombs(convPointer, input.c_str(), len + 1);
	strReturn = convPointer;
	delete [] convPointer;
	return strReturn;
}
