#ifndef PROTOCOLMANDOLYN_H
#define PROTOCOLMANDOLYN_H

#include "service/Protocol.h"
#include "service/ControllerMessage.h"

class ProtocolMandolyn : public Protocol
{
public:
	static std::string decodeData(ControllerMessage &dataMsg);
};

#endif //PROTOCOLMANDOLYN_H
