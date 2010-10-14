#pragma once
#include "Device.h"
#include <string>

class ProtocolNexa : public Protocol {
public:
	ProtocolNexa();
	virtual ~ProtocolNexa(void);

	virtual std::string getStringForMethod(int method, const std::string &data, Controller *controller);

protected:
	std::string getStringSelflearning(int method, const std::string &data);
	std::string getStringCodeSwitch(int method);
	static std::string getCodeSwitchTuple(int code);
};
