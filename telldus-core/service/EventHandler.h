#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class Event;

class EventHandler {
public:
	EventHandler();
	virtual ~EventHandler(void);

	Event *addEvent();
	bool removeEvent(Event *event);
	
	void signal(Event *event);
	bool waitForAny();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //EVENTHANDLER_H
