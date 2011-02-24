#ifndef PROTOCOLSILVANCHIP_H
#define PROTOCOLSILVANCHIP_H

#include "Protocol.h"

class ProtocolSilvanChip : public Protocol
{
public:
	int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

protected:
	virtual std::string getString(const std::string &preamble, const std::string &one, const std::string &zero, int button);
};

#endif //PROTOCOLSILVANCHIP_H
