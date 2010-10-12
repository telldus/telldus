#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class Event;
class EventBase;

class EventHandler {
public:
	EventHandler();
	virtual ~EventHandler(void);

	Event *addEvent();
	bool removeEvent(EventBase *event);
	
	bool waitForAny();

protected:
	void signal(Event *event);

private:
	class PrivateData;
	PrivateData *d;
	
	friend class Event;
};

#endif //EVENTHANDLER_H
