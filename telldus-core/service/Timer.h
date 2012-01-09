#ifndef TIMER_H
#define TIMER_H

#include "Event.h"
#include "Thread.h"

class Timer : public TelldusCore::Thread {
public:
	Timer(EventRef event);
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
