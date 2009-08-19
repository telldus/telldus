#pragma once
#include "Device.h"
#include <string.h>
#include <list>

class Controller;

namespace TelldusCore {

	typedef std::list<Device *> DeviceList;

	class DeviceGroup : public Device
	{
	public:
		DeviceGroup(int id, const std::string &model, const std::string &name);
		~DeviceGroup(void);

		virtual int methods();
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
		
		bool setDevices(const std::string &newDevices);
				
	protected:
		virtual bool setDeviceParameter(const std::string &strName, const std::string &strValue);
		
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int bell(void);
		virtual int dim(unsigned char level);		
	
	private:
		DeviceList deviceList;
	};
	
}
