#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {
	
	class DeviceX10 : public Device
	{
	public:
		DeviceX10(int id, const std::string &model, const std::string &name);
		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
		
		bool setHouse(const std::string &newHouse);
		bool setUnit(const std::string &newUnit);
		
	public:
		~DeviceX10(void);
	
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		virtual int turnOn(void);
		virtual int turnOff(void);

		int intHouse;
		int intCode;
		std::string getStringCode(bool);
	};
	
}
