#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {
	
	class DeviceNexa : public Device
	{
	public:
		DeviceNexa(int id, const std::string &model, const std::string &name);
		virtual int methods(int methodsSupported);
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
		virtual int bell(void);

		int intHouse;
		int intCode;
		std::string getStringCode(int);
	};
	
}
