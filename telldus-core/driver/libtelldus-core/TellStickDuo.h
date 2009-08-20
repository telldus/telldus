//
// C++ Interface: controller
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TELLSTICKDUO_H
#define TELLSTICKDUO_H

#include <QThread>
#include <QMutex>

#include "Controller.h"
#include "ftd2xx.h"

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
namespace TelldusCore {
	class PrivateTellStickDuoListener;
	
	class TellStickDuo : public Controller {
		friend class PrivateTellStickDuoListener;

	public:
		TellStickDuo(const std::string &serial);
	
		~TellStickDuo();
		
		virtual int firmwareVersion();
		virtual int send( const std::string &message );
		
		bool connected() const;
		
	private:
		
		FT_HANDLE ftHandle;
		bool open;
		PrivateTellStickDuoListener *listener;
		int fwVersion;
	};
}

#endif
