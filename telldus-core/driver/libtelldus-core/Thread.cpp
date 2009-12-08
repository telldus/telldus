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

#include "Thread.h"

using namespace TelldusCore;

class TelldusCore::ThreadPrivate {
public:
#ifdef _WINDOWS
	HANDLE thread;
	DWORD threadId;
	HANDLE noEvent, noWait;
#else
	pthread_t thread;
	pthread_cond_t noEvent, noWait;
#endif
	MUTEX mutex;
	
	//Must be locked by the mutex!
	bool hasEvent;
	std::string message;
};

Thread::Thread() {
	d = new ThreadPrivate;
	initMutex(&d->mutex);
	
	lockMutex(&d->mutex);
	d->hasEvent = false;
	unlockMutex(&d->mutex);
	
#ifdef _WINDOWS
	d->noEvent = CreateEvent(0, FALSE, FALSE, 0);
	d->noWait = CreateEvent(0, FALSE, FALSE, 0);
#else
	pthread_cond_init(&d->noEvent, NULL);
	pthread_cond_init(&d->noWait, NULL);
#endif
}

Thread::~Thread() {
	destroyMutex(&d->mutex);
#ifdef _WINDOWS
	CloseHandle(d->noEvent);
	CloseHandle(d->noWait);
#else
	pthread_cond_destroy(&d->noEvent);
	pthread_cond_destroy(&d->noWait);
#endif
	delete d;
}

void Thread::start() {
#ifdef _WINDOWS
	d->thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Thread::exec, this, 0, &d->threadId);
#else
	pthread_create(&d->thread, NULL, &Thread::exec, this );
#endif
}

bool Thread::wait() {
#ifdef _WINDOWS
	WaitForSingleObject(d->thread, INFINITE);
	CloseHandle(d->thread);
#else
	pthread_join(d->thread, 0);
#endif
	return true;
}

void *Thread::exec( void *ptr ) {
	Thread *t = reinterpret_cast<Thread *>(ptr);
	if (t) {
		t->run();
	}
	return 0;
}

std::string Thread::waitForEvent() {
	std::string message;
	lockMutex(&d->mutex);
	while(!d->hasEvent) {
#ifdef _WINDOWS
		unlockMutex(&d->mutex);
		WaitForSingleObject(d->noWait, INFINITE);
		lockMutex(&d->mutex);
#else
		pthread_cond_wait(&d->noWait, &d->mutex);
#endif
	}
	d->hasEvent = false;
	message = d->message;
	unlockMutex(&d->mutex);
#ifdef _WINDOWS
	SetEvent(d->noEvent);
#else
	pthread_cond_broadcast(&d->noEvent);
#endif
	return message;
}

void Thread::sendEvent(const std::string &message) {
	lockMutex(&d->mutex);
	while (d->hasEvent) { //We have an unprocessed event
#ifdef _WINDOWS
		unlockMutex(&d->mutex);
		WaitForSingleObject(d->noEvent, INFINITE);
		lockMutex(&d->mutex);
#else
		pthread_cond_wait(&d->noEvent, &d->mutex);
#endif
	}
	d->hasEvent = true;
	d->message = message;
	unlockMutex(&d->mutex);
#ifdef _WINDOWS
	SetEvent(d->noWait);
#else
	pthread_cond_broadcast(&d->noWait);
#endif
}

void Thread::initMutex(MUTEX * m) {
#ifdef _WINDOWS
	InitializeCriticalSection(m);
#else
	pthread_mutex_init(m, NULL);
#endif
}

void Thread::destroyMutex(MUTEX * m) {
#ifdef _WINDOWS
	DeleteCriticalSection(m);
#else
	pthread_mutex_destroy(m);
#endif
}

void Thread::unlockMutex(MUTEX * m) {
#ifdef _WINDOWS
	LeaveCriticalSection(m);
#else
	pthread_mutex_unlock(m);
#endif
}

void Thread::lockMutex(MUTEX * m) {
#ifdef _WINDOWS
	EnterCriticalSection(m);
#else
	pthread_mutex_lock(m);
#endif
}


MutexLocker::MutexLocker(MUTEX *m)
	:mutex(m) {
	Thread::lockMutex(mutex);
}

MutexLocker::~MutexLocker() {
	Thread::unlockMutex(mutex);
}
