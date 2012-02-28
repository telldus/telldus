//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TELLDUS_CORE_SERVICE_PROTOCOLMANDOLYN_H_
#define TELLDUS_CORE_SERVICE_PROTOCOLMANDOLYN_H_

#include "service/Protocol.h"
#include "service/ControllerMessage.h"

class ProtocolMandolyn : public Protocol
{
public:
	static std::string decodeData(ControllerMessage &dataMsg);
};

#endif  // TELLDUS_CORE_SERVICE_PROTOCOLMANDOLYN_H_
