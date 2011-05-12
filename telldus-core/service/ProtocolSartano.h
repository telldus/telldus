#ifndef PROTOCOLSARTANO_H
#define PROTOCOLSARTANO_H

#include "ControllerMessage.h"
#include "Protocol.h"

class ProtocolSartano : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
	static std::string decodeData(ControllerMessage &dataMsg);

protected:
	std::string getStringForCode(const std::wstring &code, int method);
};

#endif //PROTOCOLSARTANO_H
