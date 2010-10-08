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
	virtual ~Event();
	
	void clearSignal();
	bool isSignaled();
	EVENT_T retrieveNative();
	void signal();
	
protected:
	Event(EventHandler *handler);
	void setSignaled();
	void clearHandler();
	
private:
	class PrivateData;
	PrivateData *d;
	
friend class EventHandler;	
};

#endif //EVENT_H
