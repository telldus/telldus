#ifndef TELLDUSMAIN_H
#define TELLDUSMAIN_H

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