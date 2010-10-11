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
	typedef CRITICAL_SECTION MUTEX_T;
#else
	#include <pthread.h>
	typedef pthread_mutex_t MUTEX_T;
#endif

namespace TelldusCore {
	class ThreadPrivate;
	class Thread {
		public:
			Thread();
			virtual ~Thread();
			void start();
			bool wait();
			
			static void initMutex(MUTEX_T *m);
			static void destroyMutex(MUTEX_T *m);
			static void lockMutex(MUTEX_T *m);
			static void unlockMutex(MUTEX_T *m);
						
		protected:
			virtual void run() = 0;
		
		private:
			static void* exec( void *ptr );
			ThreadPrivate *d;
	};
	
	class MutexLocker {
	public:
		MutexLocker(MUTEX_T *m);
		~MutexLocker();
	private:
		MUTEX_T *mutex;
	};
}

#endif
