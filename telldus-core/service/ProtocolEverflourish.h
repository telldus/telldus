#ifndef PROTOCOLEVERFLOURISH_H
#define PROTOCOLEVERFLOURISH_H

#include "Protocol.h"

class ProtocolEverflourish :  public Protocol
{
public:
	virtual std::string getStringForMethod(int method, const std::string &data, Controller *controller);

private:
	static unsigned int calculateChecksum(unsigned int x);
};

#endif //PROTOCOLEVERFLOURISH_H
