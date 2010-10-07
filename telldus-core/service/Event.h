#ifndef EVENT_H
#define EVENT_H

class EventHandler;

class Event {
public:
	Event(EventHandler *handler);
	virtual ~Event();
	
	bool isSignaled();
	void signal();
	
protected:
	void setSignaled();
	
private:
	class PrivateData;
	PrivateData *d;
	
friend class EventHandler;	
};

#endif //EVENT_H
