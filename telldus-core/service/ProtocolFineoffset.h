#ifndef PROTOCOLFINEOFFSET_H
#define PROTOCOLFINEOFFSET_H

#include "service/Protocol.h"
#include "service/ControllerMessage.h"

class ProtocolFineoffset : public Protocol
{
public:
	static std::string decodeData(ControllerMessage &dataMsg);
};

#endif //PROTOCOLFINEOFFSET_H
