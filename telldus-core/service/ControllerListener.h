#ifndef CONTROLLERLISTENER_H
#define CONTROLLERLISTENER_H

#include "Thread.h"
#include "Event.h"

class ControllerChangeEventData : public EventDataBase {
public:
	int vid, pid;
	bool inserted;
};

class ControllerListener : public TelldusCore::Thread {
public:
	ControllerListener(EventRef event);
	virtual ~ControllerListener();

protected:
	void run();
	
private:	
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLERLISTENER_H
