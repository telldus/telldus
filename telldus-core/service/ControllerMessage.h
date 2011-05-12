#ifndef CONTROLLERMESSAGE_H
#define CONTROLLERMESSAGE_H

#include <string>

class ControllerMessage {
public:
	ControllerMessage(const std::string &rawMessage);
	virtual ~ControllerMessage();

	std::string msgClass() const;
	int getIntParameter(const std::string &key) const;
	std::string getParameter(const std::string &key) const;
	int method() const;
	std::wstring protocol() const;
	std::wstring model() const;

	bool hasParameter(const std::string &key) const;

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLERMESSAGE_H
