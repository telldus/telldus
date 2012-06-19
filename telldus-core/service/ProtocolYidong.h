//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PROTOCOLYIDONG_H
#define PROTOCOLYIDONG_H

#include "service/ProtocolSartano.h"

class ProtocolYidong : public ProtocolSartano
{
public:
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLYIDONG_H
