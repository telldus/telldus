
#include "Message.h"
using namespace TelldusService;
#include <sstream>
#include <ctype.h>

/*class MesssagePrivate {
public:
};*/

Message::Message()
	: std::string()
{
}

Message::Message(const std::string &functionName)
	:std::string()
{
	this->addArgument(functionName);
}

Message::~Message(void) {
	//delete d;
}

void Message::addArgument(const std::string &value) {
	std::stringstream st;
	st << (int)value.size();
	this->append(st.str());
	this->append(":");
	this->append(value);
}

void Message::addArgument(int value) {
	std::stringstream st;
	st << (int)value;
	this->append("i");
	this->append(st.str());
	this->append("e");
}

/*void Message::addArgument(const QVariant &value) {
	if (value.type() == QVariant::Int) {
		this->addArgument(value.toInt());
	} else {
		this->addArgument(value.toByteArray());
	}
}*/

void Message::addArgument(const char *value) {
	this->addArgument(std::string(value));
}

bool Message::nextIsInt(const std::string &message) {
	if (message.length() == 0) {
		return false;
	}
	return (message.at(0) == 'i');
}

bool Message::nextIsString(const std::string &message) {
	if (message.length() == 0) {
		return false;
	}
	return (isdigit(message.at(0)) != 0);
}

std::string Message::takeString(std::string *message) {
	if (!Message::nextIsString(*message)) {
		return "";
	}
	int index = message->find(':');
	int length = atoi(message->substr(0, index).c_str());
	std::string retval(message->substr(index+1, length));
	message->erase(0, index+length+1);
	return retval;
}

int Message::takeInt(std::string *message) {
	if (!Message::nextIsInt(*message)) {
		return 0;
	}
	int index = message->find('e');
	int value = atoi(message->substr(1, index - 1).c_str());
	message->erase(0, index+1);
	return value;
}