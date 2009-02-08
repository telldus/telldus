#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {
	
	class DeviceNexa : public Device
	{
	public:
		DeviceNexa(int id, int model, const std::string &strHouse, const std::string &strCode);
		virtual int methods(int methodsSupported);
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
		
	public:
		~DeviceNexa(void);
	
	protected:
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int bell(void);

		int intHouse;
		int intCode;
		std::string getStringCode(int);
	};
	
}
