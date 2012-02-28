//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TELLDUS_CORE_COMMON_MESSAGE_H_
#define TELLDUS_CORE_COMMON_MESSAGE_H_

#include <string>

namespace TelldusCore {
	class Message : public std::wstring {
	public:
		Message();
		explicit Message(const std::wstring &);
		~Message(void);

		void addArgument(const std::wstring &);
		//void addSpecialArgument(const std::wstring &);
		//void addSpecialArgument(int);
		//void addSpecialArgument(const char *);
		void addArgument(int);
		void addArgument(const char *);

		static bool nextIsInt(const std::wstring &);
		static bool nextIsString(const std::wstring &);

		static std::wstring takeString(std::wstring *);
		static int takeInt(std::wstring *);

	private:

	};
}

#endif  // TELLDUS_CORE_COMMON_MESSAGE_H_
