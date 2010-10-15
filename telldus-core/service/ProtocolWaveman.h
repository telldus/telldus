#ifndef PROTOCOLWAVEMAN_H
#define PROTOCOLWAVEMAN_H

#include "ProtocolNexa.h"

class ProtocolWaveman : public ProtocolNexa {
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

protected:
	virtual std::string getOffCode() const;
};

#endif //PROTOCOLWAVEMAN_H
