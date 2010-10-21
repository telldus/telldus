#ifndef STRING_H
#define STRING_H

#include <string>

namespace TelldusCore {
	std::wstring charToWstring(const char *value);
	bool comparei(std::wstring stringA, std::wstring stringB);
}

#endif //STRING_H
