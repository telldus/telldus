#ifndef PROTOCOLIKEA_H
#define PROTOCOLIKEA_H

#include "Protocol.h"

class ProtocolIkea : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLIKEA_H
