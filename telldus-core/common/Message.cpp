#include "Message.h"
using namespace TelldusCore;
#include <sstream>
#include <ctype.h>
#include <stdlib.h>

Message::Message()
        : std::wstring()
{
}

Message::Message(const std::wstring &functionName)
		:std::wstring()
{
        this->addArgument(functionName);
}

Message::~Message(void) {
}

void Message::addArgument(const std::wstring &value) {
        std::wstringstream st;
        st << (int)value.size();
        this->append(st.str());
        this->append(L":");
        this->append(value);
}


void Message::addArgument(int value) {
        std::wstringstream st;
        st << (int)value;
        this->append(L"i");
        this->append(st.str());
        this->append(L"e");	//TODO s?
}

/*
void Message::addArgument(const char *value) {
        this->addArgument(std::wstring(value));
}
*/



bool Message::nextIsInt(const std::wstring &message) {
        if (message.length() == 0) {
                return false;
        }
        return (message.at(0) == 'i');
}

bool Message::nextIsString(const std::wstring &message) {
        if (message.length() == 0) {
                return false;
        }
        return (isdigit(message.at(0)) != 0);
}

std::wstring Message::takeString(std::wstring *message) {
        if (!Message::nextIsString(*message)) {
                return L"";
        }
        int index = message->find(':');
        int length = wideToInteger(&message->substr(0, index));
        std::wstring retval(message->substr(index+1, length));
        message->erase(0, index+length+1);
        return retval;
}

int Message::takeInt(std::wstring *message) {
        if (!Message::nextIsInt(*message)) {
                return 0;
        }
        int index = message->find('e');	//TODO s?
        int value = wideToInteger(&message->substr(1, index - 1));
        message->erase(0, index+1);
        return value;
}

int wideToInteger(std::wstring *input){
	std::wstringstream inputstream;
	inputstream << input;
	int retval;
	inputstream >> retval;
	return retval;
}
