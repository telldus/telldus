#pragma once
#include "DeviceNexa.h"
#include <string>

namespace TelldusCore {
	
	class DeviceWaveman : public DeviceNexa
	{
	public:
		DeviceWaveman(int id, const std::string &model, const std::string &name);
		virtual int methods();
		virtual std::string getProtocol() const;
		
	protected:
		virtual int turnOff(void);
	};

}
