#include "Strings.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>

#ifdef _WINDOWS
#include <windows.h>
#else
#include <iconv.h>
#endif

#ifdef _MACOSX
#define WCHAR_T_ENCODING "UCS-4-INTERNAL"
#else
#define WCHAR_T_ENCODING "WCHAR_T"
#endif

std::wstring TelldusCore::charToWstring(const char *value) {
#ifdef _WINDOWS
	//Determine size
	int size = MultiByteToWideChar(CP_UTF8, 0, value, -1, NULL, 0);
	if (size == 0) {
		return L"";
	}
	wchar_t *buffer;
	buffer = new wchar_t[size];
	memset(buffer, 0, sizeof(wchar_t)*(size));

	int bytes = MultiByteToWideChar(CP_UTF8, 0, value, -1, buffer, size);
	std::wstring retval(buffer);
	delete[] buffer;
	return retval;

#else
	size_t utf8Length = strlen(value);
	size_t outbytesLeft = utf8Length*sizeof(wchar_t);

	//Copy the instring
	char *inString = new char[strlen(value)+1];
	strcpy(inString, value);

	//Create buffer for output
	char *outString = (char*)new wchar_t[utf8Length+1];
	memset(outString, 0, sizeof(wchar_t)*(utf8Length+1));

#ifdef _FREEBSD
	const char *inPointer = inString;
#else
	char *inPointer = inString;
#endif
	char *outPointer = outString;

	iconv_t convDesc = iconv_open(WCHAR_T_ENCODING, "UTF-8");
	iconv(convDesc, &inPointer, &utf8Length, &outPointer, &outbytesLeft);
	iconv_close(convDesc);

	std::wstring retval( (wchar_t *)outString );

	//Cleanup
	delete[] inString;
	delete[] outString;

	return retval;
#endif
}

int TelldusCore::charToInteger(const char *input){
	std::stringstream inputstream;
	inputstream << input;
	int retval;
	inputstream >> retval;
	return retval;
}

std::wstring TelldusCore::charUnsignedToWstring(const unsigned char value) {
	std::wstringstream st;
	st << value;
	return st.str();
}

/**
* This method doesn't support all locales
*/
bool TelldusCore::comparei(std::wstring stringA, std::wstring stringB) {
	transform(stringA.begin(), stringA.end(), stringA.begin(), toupper);
	transform(stringB.begin(), stringB.end(), stringB.begin(), toupper);

	return stringA == stringB;
}

std::wstring TelldusCore::intToWstring(int value) {
	std::wstringstream st;
	st << value;
	return st.str();
}

std::string TelldusCore::intToString(int value) {
	std::stringstream st;
	st << value;
	return st.str();
}

int TelldusCore::wideToInteger(const std::wstring &input){
	std::wstringstream inputstream;
	inputstream << input;
	int retval;
	inputstream >> retval;
	return retval;
}

std::string TelldusCore::wideToString(const std::wstring &input) {
#ifdef _WINDOWS
	//Determine size
	int size = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, NULL, 0, NULL, NULL);
	if (size == 0) {
		return "";
	}
	char *buffer;
	buffer = new char[size];
	memset(buffer, 0, sizeof(char)*size);

	int bytes = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, buffer, size, NULL, NULL);
	std::string retval(buffer);
	delete[] buffer;
	return retval;

#else
	size_t wideSize = sizeof(wchar_t)*input.length();
	size_t outbytesLeft = wideSize+sizeof(char); //We cannot know how many wide character there is yet

	//Copy the instring
	char *inString = (char*)new wchar_t[input.length()+1];
	memcpy(inString, input.c_str(), wideSize+sizeof(wchar_t));

	//Create buffer for output
	char *outString = new char[outbytesLeft];
	memset(outString, 0, sizeof(char)*(outbytesLeft));

#ifdef _FREEBSD
	const char *inPointer = inString;
#else
	char *inPointer = inString;
#endif
	char *outPointer = outString;

	iconv_t convDesc = iconv_open("UTF-8", WCHAR_T_ENCODING);
	iconv(convDesc, &inPointer, &wideSize, &outPointer, &outbytesLeft);
	iconv_close(convDesc);

	std::string retval(outString);

	//Cleanup
	delete[] inString;
	delete[] outString;

	return retval;
#endif
}
