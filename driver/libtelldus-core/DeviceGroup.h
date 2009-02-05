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
		DeviceGroup(int model, const std::string &strDevices);
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int bell(void);
		virtual int dim(unsigned char level);
		virtual int methods(int methodsSupported);
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
		
	public:
		~DeviceGroup(void);
	
	private:
		DeviceList deviceList;
	};
	
}
