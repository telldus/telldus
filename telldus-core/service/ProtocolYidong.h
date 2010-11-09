#ifndef PROTOCOLYIDONG_H
#define PROTOCOLYIDONG_H

#include "ProtocolSartano.h"

class ProtocolYidong : public ProtocolSartano
{
public:
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLYIDONG_H
