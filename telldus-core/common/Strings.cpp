#include "Strings.h"

#include <algorithm>
#include <sstream>
#include <string>

std::wstring TelldusCore::charToWstring(const char *value) {
	std::wstringstream st;
	st << value;
	return st.str();
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

std::string TelldusCore::wideToString(const std::wstring &input) {
	
	std::string strReturn;
	
	size_t len = input.length();
	char* convPointer = new char[len + 1];
	
	wcstombs(convPointer, input.c_str(), len + 1);
	
	strReturn = convPointer;
	
	delete [] convPointer;
	
	return strReturn;
	
}
