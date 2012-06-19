//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PROTOCOLSARTANO_H
#define PROTOCOLSARTANO_H

#include "service/Protocol.h"
#include "service/ControllerMessage.h"

class ProtocolSartano : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
	static std::string decodeData(ControllerMessage &dataMsg);

protected:
	std::string getStringForCode(const std::wstring &code, int method);
};

#endif //PROTOCOLSARTANO_H
