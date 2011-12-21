#ifndef PROTOCOLOREGON_H
#define PROTOCOLOREGON_H

#include "ControllerMessage.h"
#include "Protocol.h"

class ProtocolOregon : public Protocol
{
public:
	static std::string decodeData(ControllerMessage &dataMsg);

protected:
	static std::string decodeEA4C(const std::string &data);
	static std::string decode1A2D(const std::string &data);
};

#endif //PROTOCOLOREGON_H
