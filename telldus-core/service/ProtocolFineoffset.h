#ifndef PROTOCOLFINEOFFSET_H
#define PROTOCOLFINEOFFSET_H

#include "ControllerMessage.h"
#include "Protocol.h"

class ProtocolFineoffset : public Protocol
{
public:
	static std::string decodeData(ControllerMessage &dataMsg);
};

#endif //PROTOCOLFINEOFFSET_H
