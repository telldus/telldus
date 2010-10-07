#ifndef TELLDUSMAIN_H
#define TELLDUSMAIN_H

#include <windows.h>

class TelldusMain
{
public:
	TelldusMain(void);
	~TelldusMain(void);

	void start();
	void stop();

private:
	bool running;

};

#endif //TELLDUSMAIN_H