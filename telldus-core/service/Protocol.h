#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <map>

typedef std::map<std::wstring, std::wstring> ParameterMap;

class Protocol
{
public:
	Protocol();
	~Protocol(void);

	static Protocol *getProtocolInstance(std::wstring &protocolname);

	void setModel(const std::wstring &model);
	void setParameters(ParameterMap &parameterList);

private:
	class PrivateData;
	PrivateData *d;
	static bool comparei(std::wstring stringA , std::wstring stringB);
};

#endif //PROTOCOL_H