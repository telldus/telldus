#ifndef PROTOCOLSCENE_H
#define PROTOCOLSCENE_H

#include "service/Protocol.h"

class ProtocolScene : public Protocol {
public:
	virtual int methods() const;
	virtual std::string getStringForMethod(int method, unsigned char data, Controller *controller);

};

#endif //PROTOCOLSCENE_H



