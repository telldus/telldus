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
