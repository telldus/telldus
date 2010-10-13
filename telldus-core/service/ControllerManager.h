#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

class Controller;

class ControllerManager {
public:
	ControllerManager(void);
	~ControllerManager(void);

	Controller *getBestControllerById(int id);

protected:
	void loadControllers();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLERMANAGER_H