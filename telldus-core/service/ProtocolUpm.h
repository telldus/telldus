#ifndef PROTOCOLUPM_H
#define PROTOCOLUPM_H

#include "Protocol.h"

class ProtocolUpm : public Protocol
{
public:
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLUPM_H