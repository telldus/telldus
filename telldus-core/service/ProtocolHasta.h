#ifndef PROTOCOLHASTA_H
#define PROTOCOLHASTA_H

#include "Protocol.h"

class ProtocolHasta : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

protected:
	static std::string convertByte(unsigned char byte);
};

#endif //PROTOCOLHASTA_H
