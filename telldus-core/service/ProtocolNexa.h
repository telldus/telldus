#ifndef PROTOCOLNEXA_H
#define PROTOCOLNEXA_H

#include "Device.h"
#include <string>

class ProtocolNexa : public Protocol {
public:
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

protected:
	std::string getStringSelflearning(int method, unsigned char data);
	std::string getStringCodeSwitch(int method);
	virtual std::string getOffCode() const;
	static std::string getCodeSwitchTuple(int code);
};

#endif //PROTOCOLNEXA_H
