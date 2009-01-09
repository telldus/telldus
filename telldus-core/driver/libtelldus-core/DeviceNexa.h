#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {
	
	class DeviceNexa : public Device
	{
	public:
		DeviceNexa(int model, const std::string &strHouse, const std::string &strCode);
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int bell(void);
		virtual int methods(int methodsSupported);
		virtual std::string getProtocol() const;
		
	public:
		~DeviceNexa(void);
	
	protected:
		int intHouse;
		int intCode;
		std::string getStringCode(int);
	};
	
}
