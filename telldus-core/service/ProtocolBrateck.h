#ifndef PROTOCOLBRATECK_H
#define PROTOCOLBRATECK_H

#include "Protocol.h"

class ProtocolBrateck : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLBRATECK_H
