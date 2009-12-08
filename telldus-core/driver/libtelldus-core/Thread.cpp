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
#else
	pthread_t thread;
	pthread_cond_t noEvent, noWait;
	pthread_mutex_t mutex;
	
	//Must be locked by the mutex!
	bool hasEvent;
	std::string message;
#endif
};

Thread::Thread() {
	d = new ThreadPrivate;
#ifndef _WINDOWS
	initMutex(&d->mutex);
	
	lockMutex(&d->mutex);
	d->hasEvent = false;
	unlockMutex(&d->mutex);
	
	pthread_cond_init(&d->noEvent, NULL);
	pthread_cond_init(&d->noWait, NULL);
#endif
}

Thread::~Thread() {
#ifndef _WINDOWS
	destroyMutex(&d->mutex);
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
#ifndef _WINDOWS
		pthread_cond_wait(&d->noWait, &d->mutex);
#endif
	}
	d->hasEvent = false;
	message = d->message;
	unlockMutex(&d->mutex);
#ifndef _WINDOWS
	pthread_cond_broadcast(&d->noEvent);
#endif
	return message;
}

void Thread::sendEvent(const std::string &message) {
	lockMutex(&d->mutex);
	while (d->hasEvent) { //We have an unprocessed event
#ifndef _WINDOWS
		pthread_cond_wait(&d->noEvent, &d->mutex);
#endif
	}
	d->hasEvent = true;
	d->message = message;
	unlockMutex(&d->mutex);
#ifndef _WINDOWS
	pthread_cond_broadcast(&d->noWait);
#endif
}

void Thread::initMutex(MUTEX * m) {
#ifndef _WINDOWS
	pthread_mutex_init(m, NULL);
#endif
}

void Thread::destroyMutex(MUTEX * m) {
#ifndef _WINDOWS
	pthread_mutex_destroy(m);
#endif
}

void Thread::unlockMutex(MUTEX * m) {
#ifndef _WINDOWS
	pthread_mutex_unlock(m);
#endif
}

void Thread::lockMutex(MUTEX * m) {
#ifndef _WINDOWS
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
