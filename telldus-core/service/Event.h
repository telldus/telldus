#ifndef EVENT_H
#define EVENT_H

class EventHandler;

#ifdef _WINDOWS
	#include <windows.h>
	typedef HANDLE EVENT_T;
#else
	typedef void* EVENT_T;
#endif

class Event {
public:
	Event(EventHandler *handler);
	virtual ~Event();
	
	bool isSignaled();
	EVENT_T retrieveNative();
	void signal();
	
protected:
	void setSignaled();
	
private:
	class PrivateData;
	PrivateData *d;
	
friend class EventHandler;	
};

#endif //EVENT_H
