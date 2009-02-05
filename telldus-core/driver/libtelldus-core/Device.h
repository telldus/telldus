#pragma once

#include "telldus-core.h"
#include <string>
//#include <QMutex>

namespace TelldusCore {

	const int ALL_METHODS =
			TELLSTICK_TURNON |
			TELLSTICK_TURNOFF |
			TELLSTICK_BELL |
			TELLSTICK_DIM;

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
		virtual std::string getProtocol() const = 0;
		virtual bool parameterMatches( const std::string &name, const std::string &value ) const = 0;
		int getModel() const;
		void setModel( int intModel );
		
		static bool methodId( const std::string &methodName );
		
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
