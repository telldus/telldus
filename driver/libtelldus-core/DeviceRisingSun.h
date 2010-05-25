#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {
	
	class DeviceRisingSun : public Device
	{
	public:
		DeviceRisingSun(int id, const std::string &model, const std::string &name);
		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
		
		bool setHouse(const std::string &newHouse);
		bool setUnit(const std::string &newUnit);
		
	public:
		~DeviceRisingSun(void);
	
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int learn(void);

		int intHouse;
		int intCode;
		bool isSelflearning() const;
		
		std::string getStringCodeswitch(int);
		std::string getStringSelflearning(bool on);
	};
	
}
