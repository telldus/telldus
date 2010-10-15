#ifndef TELLDUSMAIN_H
#define TELLDUSMAIN_H

class TelldusMain
{
public:
	TelldusMain(void);
	~TelldusMain(void);

	void start();
	void stop();

	//Thread safe!
	void deviceInsertedOrRemoved(int vid, int pid, bool inserted);

private:
	class PrivateData;
	PrivateData *d;
};

#endif //TELLDUSMAIN_H
