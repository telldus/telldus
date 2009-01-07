#pragma once
#include "DeviceNexa.h"
#include <string>

namespace TelldusCore {
	
	class DeviceWaveman : public DeviceNexa
	{
	public:
		DeviceWaveman(int model, const std::string &strHouse, const std::string &strCode);
		virtual int turnOff(void);
		virtual int methods(int methodsSupported);
	};

}
