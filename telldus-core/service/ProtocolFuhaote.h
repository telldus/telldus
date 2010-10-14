#ifndef PROTOCOLFUHAOTE_H
#define PROTOCOLFUHAOTE_H

#include "Protocol.h"

class ProtocolFuhaote : public Protocol
{
public:
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLFUHAOTE_H