#ifndef PROTOCOLEVERFLOURISH_H
#define PROTOCOLEVERFLOURISH_H

#include "Protocol.h"

class ProtocolEverflourish :  public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

private:
	static unsigned int calculateChecksum(unsigned int x);
};

#endif //PROTOCOLEVERFLOURISH_H
