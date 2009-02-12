#pragma once
#include "DeviceNexa.h"
#include <string>

namespace TelldusCore {
	
	class DeviceWaveman : public DeviceNexa
	{
	public:
		DeviceWaveman(int id, int model, const std::string &name);
		virtual int methods(int methodsSupported);
		
	protected:
		virtual int turnOff(void);
	};

}
