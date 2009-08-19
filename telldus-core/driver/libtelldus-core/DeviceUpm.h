#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {

	class DeviceUpm : public Device
	{
	public:
		DeviceUpm(int id, const std::string &model, const std::string &name);
		virtual ~DeviceUpm(void);
		
		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
	
		bool setHouse(const std::string &strHouse);
		bool setUnit(const std::string &strUnit);
	
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		
		virtual int turnOn(void);
		virtual int turnOff(void);

		std::string getStringCode(bool on);
		int intHouse, intUnit;
	};

}
