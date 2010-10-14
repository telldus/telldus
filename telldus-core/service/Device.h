#ifndef DEVICE_H
#define DEVICE_H

#include "Controller.h"
#include "Mutex.h"
#include "Protocol.h"
#include <string>

class Device : public TelldusCore::Mutex
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
	int getPreferredControllerId();
	void setPreferredControllerId(int controllerId);
	std::wstring getProtocolName();
	void setProtocolName(const std::wstring &name);
	int doAction(int action, const std::wstring &data, Controller *controller);
	
private:

	Protocol *retrieveProtocol();

	class PrivateData;
	PrivateData *d;
};

#endif //DEVICE_H
