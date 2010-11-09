#ifndef DEVICEYIDONG_H
#define DEVICEYIDONG_H

#include "Device.h"
#include <string>

namespace TelldusCore {

	class DeviceYidong : public Device
	{
	public:
		DeviceYidong(int id, const std::string &model, const std::string &name);
		virtual ~DeviceYidong(void);
		virtual int methods();
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;

		virtual std::string getProtocol() const;

		bool setUnit(const std::string &newUnit);

	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		virtual int turnOn(void);
		virtual int turnOff(void);
		std::string getStringCode(void);
		int intCode;
	};

}

#endif //DEVICEYIDONG_H
