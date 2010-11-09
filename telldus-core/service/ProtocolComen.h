#ifndef PROTOCOLCOMEN_H
#define PROTOCOLCOMEN_H

#include "ProtocolNexa.h"
#include <string>

class ProtocolComen : public ProtocolNexa {
public:
	virtual int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);
};

#endif //PROTOCOLCOMEN_H
