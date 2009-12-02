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
	class PrivateVars;
	
	class TellStick : public Controller {
	public:	
		virtual ~TellStick();
		
		virtual int firmwareVersion();
		virtual int send( const std::string &message );

		static TellStick *findFirstDevice();
		
	protected:
		TellStick(int vid, int pid );
		void setBaud( int baud );
		
	private:
		static std::string findByVIDPID( int vid, int pid );
		
		PrivateVars *d;
	};
}

#endif
