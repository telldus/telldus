#include "DeviceUndefined.h"

using namespace TelldusCore;

/*
* Constructor
*/
DeviceUndefined::DeviceUndefined(int id, const std::string &model, const std::string &name)
	:Device(id, model, name)
{
}

/*
* Destructor
*/
DeviceUndefined::~DeviceUndefined(void)
{}

bool DeviceUndefined::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	return true;
}

bool DeviceUndefined::parameterMatches( const std::string &name, const std::string &value ) const {
	return false;
}

/*
* Has the device got the method?
*/
int DeviceUndefined::methods(){
	return 0;
}

std::string DeviceUndefined::getProtocol() const {
	return "undefined";
}


int TelldusCore::DeviceUndefined::sendRawCommand(const std::string & strMessage) {
	return send(strMessage);
}
