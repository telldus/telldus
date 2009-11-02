//
// C++ Interface: TellStick
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TELLSTICK_H
#define TELLSTICK_H

#include "Controller.h"

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
namespace TelldusCore {
	class TellStick : public Controller {
	public:	
		virtual ~TellStick();
		
		static int findFirstDevice();
		
	protected:
		TellStick();
		
	private:
		static std::string findByVIDPID( int vid, int pid );
	};
}

#endif
