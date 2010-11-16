#ifndef PROTOCOLGROUP_H
#define PROTOCOLGROUP_H

#include "Protocol.h"

class ProtocolGroup : public Protocol {
public:
	virtual int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

};

#endif //PROTOCOLGROUP_H



