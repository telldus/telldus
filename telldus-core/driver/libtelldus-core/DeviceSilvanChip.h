#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {

	class DeviceSilvanChip : public Device
	{
	public:
		DeviceSilvanChip(int id, const std::string &model, const std::string &name);
		virtual ~DeviceSilvanChip(void);
		
		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
	
		bool setHouse(const std::string &strNewHouse);
	
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		
		virtual int turnOn(Controller *controller);
		virtual int turnOff(Controller *controller);
		virtual int learn(Controller *controller);

		std::string getStringCode(Controller *controller, int method);
		int intHouse;
	};

}
