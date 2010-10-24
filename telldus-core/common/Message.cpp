#include "Message.h"
#include "Socket.h"
#include "Strings.h"
#include <sstream>
#include <wctype.h>
#include <stdlib.h>

using namespace TelldusCore;


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
	this->append(L"s");
}

void Message::addArgument(const char *value) {
	this->addArgument(TelldusCore::charToWstring(value));
}

std::wstring Message::charUnsignedToWstring(const unsigned char value) {
	//todo move?
	std::wstringstream st;
	st << value;
	return st.str();
}

std::wstring Message::intToWstring(int value) {
	//todo	move?
	std::wstringstream st;
	st << value;
	return st.str();
}

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
	return (iswdigit(message.at(0)) != 0);
}

std::wstring Message::takeString(std::wstring *message) {
    
	if (!Message::nextIsString(*message)) {
		return L"";
	}
	size_t index = message->find(':');
	int length = wideToInteger(message->substr(0, index));
	std::wstring retval(message->substr(index+1, length));
	message->erase(0, index+length+1);
	return retval;
}

int Message::takeInt(std::wstring *message) {
	if (!Message::nextIsInt(*message)) {
		return 0;
	}
	size_t index = message->find('s');
	int value = wideToInteger(message->substr(1, index - 1));
	message->erase(0, index+1);
	return value;
}

int Message::wideToInteger(const std::wstring &input){
	std::wstringstream inputstream;
	inputstream << input;
	int retval;
	inputstream >> retval;
	return retval;
}
