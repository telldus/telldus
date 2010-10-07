#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "Event.h"

class EventHandler {
public:
	EventHandler();
	virtual ~EventHandler(void);

	void addEvent(EVENT event);
	EVENT addEvent();

	EVENT waitForAny();

	static EVENT createEvent();
private:
	class PrivateData;
	PrivateData *d;
};

#endif //EVENTHANDLER_H
