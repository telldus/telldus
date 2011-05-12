#ifndef PROTOCOLX10_H
#define PROTOCOLX10_H

#include "Protocol.h"
#include "ControllerMessage.h"

class ProtocolX10 : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

	static std::string decodeData(ControllerMessage& dataMsg);
};

#endif //PROTOCOLX10_H
