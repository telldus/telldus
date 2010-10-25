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

	static Protocol *getProtocolInstance(const std::wstring &protocolname);

	virtual int methods() const = 0;
	std::wstring model() const;
	void setModel(const std::wstring &model);
	void setParameters(ParameterMap &parameterList);

	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller) = 0;

protected:
	std::wstring getStringParameter(const std::wstring &name, const std::wstring &defaultValue = L"") const;
	int getIntParameter(const std::wstring &name, int min, int max) const;

private:
	class PrivateData;
	PrivateData *d;
};

#endif //PROTOCOL_H