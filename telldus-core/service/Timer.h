//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TIMER_H
#define TIMER_H

#include "common/Event.h"
#include "common/Thread.h"

class Timer : public TelldusCore::Thread {
public:
	Timer(TelldusCore::EventRef event);
	virtual ~Timer();

	void setInterval(int sec);
	void stop();

protected:
	void run();

private:
	class PrivateData;
	PrivateData *d;
};


#endif //TIMER_H
