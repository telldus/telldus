#ifndef PROTOCOLNEXA_H
#define PROTOCOLNEXA_H

#include "Device.h"
#include <string>

class ProtocolNexa : public Protocol {
public:
	virtual int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

protected:
	std::string getStringSelflearning(int method, unsigned char data);
	std::string getStringCodeSwitch(int method);
	std::string getStringBell();
	virtual std::string getOffCode() const;
	static std::string getCodeSwitchTuple(int code);
	static std::string getStringSelflearningForCode(int house, int unit, int method, unsigned char data);
};

#endif //PROTOCOLNEXA_H
