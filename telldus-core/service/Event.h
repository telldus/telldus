#ifndef EVENT_H
#define EVENT_H

class EventHandler;
#include "Thread.h"

#ifdef _WINDOWS
	#include <windows.h>
	typedef HANDLE EVENT_T;
#else
	typedef void* EVENT_T;
#endif

class EventData {
public:
	bool isValid() const;
};

class EventDataBase : public EventData {
public:
	bool isValid() const;
};

class EventBase {
public:
	virtual ~EventBase();
	
	bool isSignaled();
	void signal();
	virtual void signal(EventData *);
	EventData *takeSignal();

protected:
	EventBase(EventHandler *handler);
	void clearHandler();
	virtual void clearSignal() = 0;
	virtual void sendSignal() = 0;

private:
	class PrivateData;
	PrivateData *d;
};

class Event : public EventBase {
public:
	virtual ~Event();

protected:
	Event(EventHandler *handler);
	EVENT_T retrieveNative();
	virtual void clearSignal();
	virtual void sendSignal();
	
private:
	class PrivateData;
	PrivateData *d;
	
friend class EventHandler;	
};

#endif //EVENT_H
