//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONTROLLERLISTENER_H
#define CONTROLLERLISTENER_H

#include "common/Thread.h"
#include "common/Event.h"

class ControllerChangeEventData : public TelldusCore::EventDataBase {
public:
	int vid, pid;
	bool inserted;
};

class ControllerListener : public TelldusCore::Thread {
public:
	ControllerListener(TelldusCore::EventRef event);
	virtual ~ControllerListener();

protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLERLISTENER_H
