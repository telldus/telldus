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

#include "TellStick.h"
#include "ftd2xx.h"

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
namespace TelldusCore {
	class PrivateTellStickDuoListener;
	class TellStickDuoPrivate;
	
	class TellStickDuo : public TellStick {
		friend class PrivateTellStickDuoListener;
 		friend class TellStick;

	public:
		~TellStickDuo();
		
// 		virtual int firmwareVersion();
//		virtual int send( const std::string &message );
		
		bool connected() const;
		
	protected:
		TellStickDuo(const TellStickDescriptor &d);
		
	private:
		TellStickDuoPrivate *d;
	};
}

#endif
