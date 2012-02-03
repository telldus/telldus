#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "Event.h"

class EventHandler {
public:
	EventHandler();
	virtual ~EventHandler(void);

	EventRef addEvent();

	bool waitForAny();

protected:
	void signal(Event *event);

private:
	class PrivateData;
	PrivateData *d;
	bool listIsSignalled();

	friend class Event;
};

#endif //EVENTHANDLER_H
