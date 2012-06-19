//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PROTOCOLWAVEMAN_H
#define PROTOCOLWAVEMAN_H

#include "service/ProtocolNexa.h"

class ProtocolWaveman : public ProtocolNexa {
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
	static std::string decodeData(ControllerMessage& dataMsg);

protected:
	virtual std::string getOffCode() const;

private:
	static int lastArctecCodeSwitchWasTurnOff;
};

#endif //PROTOCOLWAVEMAN_H
