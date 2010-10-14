#ifndef PROTOCOLBRATECK_H
#define PROTOCOLBRATECK_H

#include "Protocol.h"

class ProtocolBrateck :  public Protocol
{
public:
	virtual std::string getStringForMethod(int method, const std::string &data, Controller *controller);
};

#endif //PROTOCOLBRATECK_H