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
};

Thread::Thread() {
	d = new ThreadPrivate;
	d->thread = 0;
}

Thread::~Thread() {
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
	if (!d->thread) {
		return true;
	}
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

void Thread::initMutex(MUTEX_T * m) {
#ifdef _WINDOWS
	InitializeCriticalSection(m);
#else
	pthread_mutex_init(m, NULL);
#endif
}

void Thread::destroyMutex(MUTEX_T * m) {
#ifdef _WINDOWS
	DeleteCriticalSection(m);
#else
	pthread_mutex_destroy(m);
#endif
}

void Thread::unlockMutex(MUTEX_T * m) {
#ifdef _WINDOWS
	LeaveCriticalSection(m);
#else
	pthread_mutex_unlock(m);
#endif
}

void Thread::lockMutex(MUTEX_T * m) {
#ifdef _WINDOWS
	EnterCriticalSection(m);
#else
	pthread_mutex_lock(m);
#endif
}


MutexLocker::MutexLocker(MUTEX_T *m)
	:mutex(m) {
	Thread::lockMutex(mutex);
}

MutexLocker::~MutexLocker() {
	Thread::unlockMutex(mutex);
}
