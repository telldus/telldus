#pragma once

#include "telldus-core.h"
#include <string>
//#include <QMutex>

namespace TelldusCore {

	class Device
	{
	public:
		Device(int model);
		~Device(void);
		
		virtual int turnOn(void);
		virtual int turnOff(void);
		virtual int bell(void);
		virtual int dim(unsigned char level);
		virtual int methods(int methodsSupported) = 0;
		virtual std::string getProtocol() = 0;
		int getModel();
		
	#ifdef _LINUX
		void setDevice(const std::string &device);
	protected:
		std::string strDevice;
	#endif
	
	protected:
		int send(const std::string &strMessage);
	
	private:
		int model;
	//	static QMutex deviceMutex;
	};

}
