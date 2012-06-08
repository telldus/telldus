//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TELLDUS_CORE_SERVICE_PROTOCOLNEXA_H_
#define TELLDUS_CORE_SERVICE_PROTOCOLNEXA_H_

#include <stdint.h>
#include <string>
#include "service/ControllerMessage.h"
#include "service/Device.h"

class ProtocolNexa : public Protocol {
public:
	virtual int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
	static std::string decodeData(const ControllerMessage &dataMsg);

protected:
	std::string getStringSelflearning(int method, unsigned char data);
	std::string getStringCodeSwitch(int method);
	std::string getStringBell();
	virtual std::string getOffCode() const;
	static std::string getCodeSwitchTuple(int code);
	static std::string getStringSelflearningForCode(int house, int unit, int method, unsigned char data);

private:
	static int lastArctecCodeSwitchWasTurnOff;
	static std::string decodeDataCodeSwitch(uint32_t allData);
	static std::string decodeDataSelfLearning(uint32_t allData);
};

#endif  // TELLDUS_CORE_SERVICE_PROTOCOLNEXA_H_
