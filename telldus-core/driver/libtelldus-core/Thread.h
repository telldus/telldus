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

#ifdef _WINDOWS
	#include <windows.h>
	typedef CRITICAL_SECTION MUTEX;
#else
	#include <pthread.h>
	typedef pthread_mutex_t MUTEX;
#endif

namespace TelldusCore {
	class ThreadPrivate;
	class Thread {
		public:
			Thread();
			~Thread();
			void start();
			bool wait();
			
			void sendEvent(const std::string &string);
			
			static void initMutex(MUTEX *m);
			static void destroyMutex(MUTEX *m);
			static void lockMutex(MUTEX *m);
			static void unlockMutex(MUTEX *m);
						
		protected:
			virtual void run() = 0;
			std::string waitForEvent();
		
		private:
			static void* exec( void *ptr );
			ThreadPrivate *d;
	};
	
	class MutexLocker {
	public:
		MutexLocker(MUTEX *m);
		~MutexLocker();
	private:
		MUTEX *mutex;
	};
}

#endif
