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
		virtual int turnOn(Controller *controller);
		virtual int turnOff(Controller *controller);
		virtual int dim(unsigned char level, Controller *controller);
		virtual int bell(Controller *controller);
		virtual int learn(Controller *controller);
		
		bool isSelflearning() const;
		bool isDimmer() const;

		int intHouse;
		int intCode;
		std::string getStringCodeSwitch(int);
		std::string getStringSelflearning(bool dim, unsigned char level);
	};
	
}
