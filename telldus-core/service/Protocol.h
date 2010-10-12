#ifndef PROTOCOL_H
#define PROTOCOL_H

class Protocol
{
public:
	Protocol();
	~Protocol(void);

private:
	class PrivateData;
	PrivateData *d;
};

#endif //PROTOCOL_H