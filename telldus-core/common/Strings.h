#ifndef STRING_H
#define STRING_H

#include <string>

namespace TelldusCore {
	std::wstring charToWstring(const char *value);
	int charToInteger(const char *value);
	std::wstring charUnsignedToWstring(const unsigned char value);

	bool comparei(std::wstring stringA, std::wstring stringB);
	std::wstring intToWstring(int value);
	std::string intToString(int value);
	std::string wideToString(const std::wstring &input);

	int wideToInteger(const std::wstring &input);
}

#endif //STRING_H
