#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {
	
	class DeviceNexa : public Device
	{
	public:
		DeviceNexa(int id, const std::string &model, const std::string &name);
		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
		
		bool setHouse(const std::string &newHouse);
		bool setUnit(const std::string &newUnit);
		
	public:
		~DeviceNexa(void);
	
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int dim(unsigned char level);
		virtual int bell(void);
		virtual int learn(void);
		
		bool isSelflearning() const;
		bool isDimmer() const;

		int intHouse;
		int intCode;
		std::string getStringCodeSwitch(int);
		std::string getStringSelflearning(bool dim, unsigned char level);
	};
	
}
