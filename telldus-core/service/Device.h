#ifndef DEVICE_H
#define DEVICE_H

#include "Controller.h"
#include "Mutex.h"
#include "Protocol.h"
#include <string>
#include <list>

class Device : public TelldusCore::Mutex
{
public:
	Device(int id);
	~Device(void);

	int doAction(int action, unsigned char data, Controller *controller);
	std::wstring getStateValue();
	int getLastSentCommand(int methodsSupported);
	int getMethods() const;
	std::wstring getModel();
	void setModel(const std::wstring &model);
	std::wstring getName();
	void setName(const std::wstring &name);
	std::wstring getParameter(const std::wstring &key);
	std::list<std::string> getParametersForProtocol() const;
	void setParameter(const std::wstring &key, const std::wstring &value);
	int getPreferredControllerId();
	void setPreferredControllerId(int controllerId);
	std::wstring getProtocolName() const;
	void setProtocolName(const std::wstring &name);
	void setStateValue(int stateValue);
	void setLastSentCommand(int command, std::wstring value);
	int getType();

	static int maskUnsupportedMethods(int methods, int supportedMethods);
	static int methodId( const std::string &methodName );
	
private:
	Protocol *retrieveProtocol() const;

	class PrivateData;
	PrivateData *d;
};

#endif //DEVICE_H
