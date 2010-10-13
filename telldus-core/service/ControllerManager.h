#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

class ControllerManager {
public:
	ControllerManager(void);
	~ControllerManager(void);

protected:
	void loadControllers();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLERMANAGER_H