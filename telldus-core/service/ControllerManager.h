#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

class Controller;
class Event;

class ControllerManager {
public:
	ControllerManager(Event *event);
	~ControllerManager(void);

	void deviceInsertedOrRemoved(int vid, int pid, bool inserted);

	Controller *getBestControllerById(int id);

protected:
	void loadControllers();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLERMANAGER_H