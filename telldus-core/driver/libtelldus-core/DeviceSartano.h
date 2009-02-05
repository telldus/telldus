#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {

	class DeviceSartano : public Device
	{
	public:
		DeviceSartano(int model, const std::string &strCode);
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int methods(int methodsSupported);
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
	
		~DeviceSartano(void);
	
	protected:
		std::string getStringCode(void);
		std::string strCode;
	};

}
