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
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

/**
	@author Micke Prag <micke.prag@telldus.se>
*/
namespace TelldusCore {
	class Controller {
	public:	
		virtual ~Controller();
		
		virtual int firmwareVersion() = 0;
		virtual int send( const std::string &message ) = 0;
		
	protected:
		Controller();
	};
}

#endif
