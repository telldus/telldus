#ifndef PROTOCOLSARTANO_H
#define PROTOCOLSARTANO_H

#include "Protocol.h"

class ProtocolSartano : public Protocol
{
public:
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLSARTANO_H