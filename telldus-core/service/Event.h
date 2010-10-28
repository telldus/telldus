#ifndef EVENT_H
#define EVENT_H

class EventHandler;
#include "Thread.h"

#ifdef _WINDOWS
	#include <windows.h>
	#include <memory>
	typedef HANDLE EVENT_T;
#else
	#include <tr1/memory>
	typedef void* EVENT_T;
#endif

class EventData {
public:
	virtual ~EventData();
	virtual bool isValid() const;
};

class EventDataBase : public EventData {
public:
	virtual bool isValid() const;
};

typedef std::tr1::shared_ptr<EventData> EventDataRef;

class EventBase {
public:
	virtual ~EventBase();

	void popSignal();
	bool isSignaled();
	void signal();
	virtual void signal(EventData *);
	EventDataRef takeSignal();

protected:
	EventBase(EventHandler *handler);
	void clearHandler();
	virtual void clearSignal() = 0;
	EventHandler *handler() const;
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

typedef std::tr1::shared_ptr<Event> EventRef;

#endif //EVENT_H
