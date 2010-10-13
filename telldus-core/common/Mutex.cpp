//
// C++ Implementation: Thread
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "Mutex.h"

using namespace TelldusCore;


#ifdef _WINDOWS
	#include <windows.h>
	typedef CRITICAL_SECTION MUTEX_T;
#else
	#include <pthread.h>
	typedef pthread_mutex_t MUTEX_T;
#endif

class Mutex::PrivateData {
public:
	MUTEX_T mutex;
};

Mutex::Mutex() {
	d = new PrivateData;
#ifdef _WINDOWS
	InitializeCriticalSection(&d->mutex);
#else
	pthread_mutex_init(&d->mutex, NULL);
#endif
}

Mutex::~Mutex() {
#ifdef _WINDOWS
	DeleteCriticalSection(&d->mutex);
#else
	pthread_mutex_destroy(&d->mutex);
#endif
	delete d;
}
			
void Mutex::lock() {
#ifdef _WINDOWS
	EnterCriticalSection(&d->mutex);
#else
	pthread_mutex_lock(&d->mutex);
#endif
}

void Mutex::unlock() {
#ifdef _WINDOWS
	LeaveCriticalSection(&d->mutex);
#else
	pthread_mutex_unlock(&d->mutex);
#endif
}


MutexLocker::MutexLocker(Mutex *m)
	:mutex(m) 
{
	mutex->lock();
}

MutexLocker::~MutexLocker() {
	mutex->unlock();
}
