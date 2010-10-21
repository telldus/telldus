#include "ControllerMessage.h"
#include "Device.h"
#include "Strings.h"

#include <map>

class ControllerMessage::PrivateData {
public:
	std::map<std::string, std::string> parameters;
	std::string protocol;
	int method;
};

ControllerMessage::ControllerMessage(const std::string &message) {
	d = new PrivateData;

	//Process our message into bits
	size_t prevPos = 0;
	size_t pos = message.find(";");
	while(pos != std::string::npos) {
		std::string param = message.substr(prevPos, pos-prevPos);
		prevPos = pos+1;
		size_t delim = param.find(":");
		if (delim == std::string::npos) {
			break;
		}
		if (param.substr(0, delim).compare("protocol") == 0) {
			d->protocol = param.substr(delim+1, param.length()-delim);
		} else if (param.substr(0, delim).compare("method") == 0) {
			d->method = Device::methodId(param.substr(delim+1, param.length()-delim));
		} else {
			d->parameters[param.substr(0, delim)] = param.substr(delim+1, param.length()-delim);
		}
		pos = message.find(";", pos+1);
	}
}

ControllerMessage::~ControllerMessage(){
	delete d;
}

int ControllerMessage::method() const {
	return d->method;
}

std::wstring ControllerMessage::protocol() const {
	return TelldusCore::charToWstring(d->protocol.c_str());
}