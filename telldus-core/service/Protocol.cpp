#include "Protocol.h"

class Protocol::PrivateData {
public:

};

Protocol::Protocol(){

	d = new PrivateData;
	
}

Protocol::~Protocol(void) {
	delete d;
}
