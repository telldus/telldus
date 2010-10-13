#ifndef DEVICE_H
#define DEVICE_H

#include "Controller.h"
#include "Protocol.h"
#include <string>

class Device
{
public:
	Device(int id);
	~Device(void);

	std::wstring getModel();
	void setModel(const std::wstring &model);
	std::wstring getName();
	void setName(const std::wstring &name);
	std::wstring getParameter(const std::wstring &key);
	void setParameter(const std::wstring &key, const std::wstring &value);
	std::wstring getProtocolName();
	void setProtocolName(const std::wstring &name);
	
	
	int turnOn(Controller *controller);

private:

	Protocol *retrieveProtocol();

	class PrivateData;
	PrivateData *d;
};

#endif //DEVICE_H
