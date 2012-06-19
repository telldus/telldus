//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PROTOCOLGROUP_H
#define PROTOCOLGROUP_H

#include "service/Protocol.h"

class ProtocolGroup : public Protocol {
public:
	virtual int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

};

#endif //PROTOCOLGROUP_H



