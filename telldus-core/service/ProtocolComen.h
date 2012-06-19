//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PROTOCOLCOMEN_H
#define PROTOCOLCOMEN_H

#include "service/ProtocolNexa.h"
#include <string>

class ProtocolComen : public ProtocolNexa {
public:
	virtual int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLCOMEN_H
