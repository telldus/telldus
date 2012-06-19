//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PROTOCOLBRATECK_H
#define PROTOCOLBRATECK_H

#include "service/Protocol.h"

class ProtocolBrateck : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLBRATECK_H
