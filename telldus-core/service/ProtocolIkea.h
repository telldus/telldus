#ifndef PROTOCOLIKEA_H
#define PROTOCOLIKEA_H

#include "Protocol.h"

class ProtocolIkea : public Protocol
{
public:
	virtual std::string getStringForMethod(int method, const std::string &data, Controller *controller);
};

#endif //PROTOCOLIKEA_H