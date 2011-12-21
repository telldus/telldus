#ifndef PROTOCOLMANDOLYN_H
#define PROTOCOLMANDOLYN_H

#include "ControllerMessage.h"
#include "Protocol.h"

class ProtocolMandolyn : public Protocol
{
public:
	static std::string decodeData(ControllerMessage &dataMsg);
};

#endif //PROTOCOLMANDOLYN_H
