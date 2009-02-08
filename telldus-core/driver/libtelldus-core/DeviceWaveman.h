#pragma once
#include "DeviceNexa.h"
#include <string>

namespace TelldusCore {
	
	class DeviceWaveman : public DeviceNexa
	{
	public:
		DeviceWaveman(int id, int model, const std::string &strHouse, const std::string &strCode);
		virtual int methods(int methodsSupported);
		
	protected:
		virtual int turnOff(void);
	};

}
