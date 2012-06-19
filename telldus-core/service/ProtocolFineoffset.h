//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
