#ifndef PROTOCOLFUHAOTE_H
#define PROTOCOLFUHAOTE_H

#include "service/Protocol.h"

class ProtocolFuhaote : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLFUHAOTE_H
