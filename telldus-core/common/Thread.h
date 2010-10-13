//
// C++ Interface: Thread
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef THREAD_H
#define THREAD_H

#include <string>

namespace TelldusCore {
	class ThreadPrivate;
	class Thread {
		public:
			Thread();
			virtual ~Thread();
			void start();
			bool wait();
						
		protected:
			virtual void run() = 0;
		
		private:
			static void* exec( void *ptr );
			ThreadPrivate *d;
	};
}

#endif
