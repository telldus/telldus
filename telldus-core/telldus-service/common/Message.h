#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

//class MessagePrivate;
namespace TelldusService {
	class Message : public std::string {
	public:
		Message();
		Message(const std::string &);
		~Message(void);
	
		void addArgument(const std::string &);
		void addArgument(int);
		//void addArgument(const QVariant &);
		void addArgument(const char *);
	
		static bool Message::nextIsInt(const std::string &);
		static bool Message::nextIsString(const std::string &);

		static std::string takeString(std::string *);
		static int takeInt(std::string *);
	
		//MessagePrivate *d;
	};
}

#endif //MESSAGE_H
