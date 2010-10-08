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
	class PrivateData;
	PrivateData *d;
};

#endif //TELLDUSMAIN_H
