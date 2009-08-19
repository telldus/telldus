#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {
	
	class DeviceUndefined : public Device
	{
	public:
		DeviceUndefined(int id, const std::string &model, const std::string &name);
		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
		
	public:
		~DeviceUndefined(void);
	
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
	};
	
}
