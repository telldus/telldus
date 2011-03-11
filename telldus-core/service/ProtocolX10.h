#ifndef PROTOCOLX10_H
#define PROTOCOLX10_H

#include "Protocol.h"

class ProtocolX10 : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLX10_H
