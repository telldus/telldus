#pragma once
#include "Device.h"
#include <string>

class ProtocolNexa : public Protocol {
public:
	ProtocolNexa();
	virtual ~ProtocolNexa(void);

	virtual std::string getStringForMethod(int method, const std::string &data, Controller *controller);
		
};
