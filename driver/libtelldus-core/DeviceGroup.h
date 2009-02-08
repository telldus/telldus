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
		DeviceGroup(int id, int model, const std::string &strDevices);
		~DeviceGroup(void);

		virtual int methods(int methodsSupported);
		virtual std::string getProtocol() const;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const;
				
	protected:
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int bell(void);
		virtual int dim(unsigned char level);		
	
	private:
		DeviceList deviceList;
	};
	
}
