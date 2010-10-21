#include "Strings.h"

#include <algorithm>
#include <sstream>
#include <string>

std::wstring TelldusCore::charToWstring(const char *value) {
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
