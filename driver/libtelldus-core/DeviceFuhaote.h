#ifndef DEVICEFUHAOTE_H
#define DEVICEFUHAOTE_H

#include "Device.h"
#include <string>

namespace TelldusCore {

	class DeviceFuhaote : public Device
	{
	public:
		DeviceFuhaote(int id, const std::string &model, const std::string &name);
		virtual ~DeviceFuhaote(void);
		
		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
	
		bool setCode(const std::string &strNewCode);
	
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		
		virtual int turnOn(void);
		virtual int turnOff(void);

		std::string getStringCode(void);
		std::string strCode;
	};

}

#endif //DEVICEFUHAOTE_H