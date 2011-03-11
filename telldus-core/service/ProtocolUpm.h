#ifndef PROTOCOLUPM_H
#define PROTOCOLUPM_H

#include "Protocol.h"

class ProtocolUpm : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLUPM_H
