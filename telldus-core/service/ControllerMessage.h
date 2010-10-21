#ifndef CONTROLLERMESSAGE_H
#define CONTROLLERMESSAGE_H

#include <string>

class ControllerMessage {
public: 
	ControllerMessage(const std::string &rawMessage);
	virtual ~ControllerMessage();

	int method() const;
	std::wstring protocol() const;

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLERMESSAGE_H