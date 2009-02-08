#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {

	class DeviceIkea : public Device
	{
	public:
		DeviceIkea(int id, int model, const std::string &strSystem, const std::string &strUnits, const std::string &strFadeStyle);
		virtual int methods(int methodsSupported);
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
		
	public:
		~DeviceIkea(void);
	
	protected:
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int dim(unsigned char level);

		int intSystem;
		int intUnits;
		int intFadeStyle;
		std::string getStringCode(unsigned char);
	};
	
}
