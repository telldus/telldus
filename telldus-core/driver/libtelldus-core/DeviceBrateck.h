#pragma once
#include "Device.h"
#include <string>

namespace TelldusCore {

	class DeviceBrateck : public Device
	{
	public:
		DeviceBrateck(int id, const std::string &model, const std::string &name);
		virtual ~DeviceBrateck(void);
		
		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
	
		bool setHouse(const std::string &strNewHouse);
	
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		
		virtual int turnOn(void);
		virtual int turnOff(void);

		std::string getStringCode(void);
		std::string strHouse;
	};

}
