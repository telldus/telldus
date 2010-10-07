#ifndef EVENT_H
#define EVENT_H

#ifdef _WINDOWS
	#include <windows.h>
	typedef HANDLE EVENT;
#else
	#include <pthread.h>
	typedef pthread_cond_t EVENT;
#endif

#endif //EVENT_H
