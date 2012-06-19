//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
