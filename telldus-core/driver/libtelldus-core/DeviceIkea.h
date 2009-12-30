#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {

	class DeviceIkea : public Device
	{
	public:
		DeviceIkea(int id, const std::string &model, const std::string &name);
		virtual ~DeviceIkea(void);
		
		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
		
		bool setSystem(const std::string &strSystem);
		bool setUnits(const std::string &strUnits);
		bool setFade(const std::string &strFade);
	
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		
		virtual int turnOn(Controller *controller);
		virtual int turnOff(Controller *controller);
		virtual int dim(unsigned char level, Controller *controller);

		int intSystem;
		int intUnits;
		int intFadeStyle;
		std::string getStringCode(unsigned char);
	};
	
}
