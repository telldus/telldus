#include "Strings.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <iconv.h>

std::wstring TelldusCore::charToWstring(const char *value) {
	size_t utf8Length = strlen(value);
	size_t outbytesLeft = utf8Length*sizeof(wchar_t);

	//Copy the instring
	char *inString = new char[strlen(value)+1];
	strcpy(inString, value);

	//Create buffer for output
	char *outString = (char*)new wchar_t[utf8Length+1];
	memset(outString, 0, sizeof(wchar_t)*(utf8Length+1));

	char *inPointer = inString;
	char *outPointer = outString;

	iconv_t convDesc = iconv_open("WCHAR_T", "UTF-8");
	iconv(convDesc, &inPointer, &utf8Length, &outPointer, &outbytesLeft);
	iconv_close(convDesc);

	std::wstring retval( (wchar_t *)outString );

	//Cleanup
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
	size_t wideSize = sizeof(wchar_t)*input.length();
	size_t outbytesLeft = wideSize+sizeof(char); //We cannot know how many wide character there is yet

	//Copy the instring
	char *inString = (char*)new wchar_t[input.length()+1];
	memcpy(inString, input.c_str(), wideSize+sizeof(wchar_t));

	//Create buffer for output
	char *outString = new char[outbytesLeft];
	memset(outString, 0, sizeof(char)*(outbytesLeft));

	char *inPointer = inString;
	char *outPointer = outString;

	iconv_t convDesc = iconv_open("UTF-8", "WCHAR_T");
	size_t converted = iconv(convDesc, &inPointer, &wideSize, &outPointer, &outbytesLeft);
	iconv_close(convDesc);

	std::string retval(outString);

	//Cleanup
	delete[] inString;
	delete[] outString;

	return retval;
}
