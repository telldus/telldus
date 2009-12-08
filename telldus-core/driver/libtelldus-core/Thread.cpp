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
#else
	pthread_t thread;
#endif
};

Thread::Thread() {
	d = new ThreadPrivate;
}

Thread::~Thread() {
	delete d;
}

void Thread::start() {
#ifndef _WINDOWS
	pthread_create(&thread, NULL, &Thread::exec, this );
#endif
}

bool Thread::wait() {
#ifndef _WINDOWS
	pthread_join(thread, 0);
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
