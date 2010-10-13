//
// C++ Interface: Thread
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MUTEX_H
#define MUTEX_H

namespace TelldusCore {
	class Mutex {
	public:
		Mutex();
		virtual ~Mutex();
			
		void lock();
		void unlock();
						
	private:
		class PrivateData;
		PrivateData *d;
	};
	
	class MutexLocker {
	public:
		MutexLocker(Mutex *m);
		~MutexLocker();
	private:
		Mutex *mutex;
	};
}

#endif //MUTEX_H
