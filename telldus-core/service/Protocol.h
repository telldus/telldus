#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <map>
#include "../client/telldus-core.h"

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

	virtual std::string getStringForMethod(int method, const std::string &data, Controller *controller) = 0;

protected:
	std::wstring getStringParameter(const std::wstring &name, const std::wstring &defaultValue = L"") const;
	int getIntParameter(const std::wstring &name, int min, int max) const;

private:
	class PrivateData;
	PrivateData *d;
	static bool comparei(std::wstring stringA , std::wstring stringB);
};

#endif //PROTOCOL_H