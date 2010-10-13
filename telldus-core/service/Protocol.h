#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <map>

typedef std::map<std::wstring, std::wstring> ParameterMap;

class Controller;

class Protocol
{
public:
	Protocol();
	virtual ~Protocol(void);

	static Protocol *getProtocolInstance(std::wstring &protocolname);

	void setModel(const std::wstring &model);
	void setParameters(ParameterMap &parameterList);

	virtual int turnOn(Controller *controller);
	virtual int turnOff(Controller *controller);
	virtual int bell(Controller *controller);
	virtual int dim(unsigned char level, Controller *controller);
	virtual int learn(Controller *controller);

private:
	class PrivateData;
	PrivateData *d;
	static bool comparei(std::wstring stringA , std::wstring stringB);
};

#endif //PROTOCOL_H