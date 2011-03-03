#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

class Controller;
class Event;

#include <string>

class ControllerManager {
public:
	ControllerManager(Event *event);
	~ControllerManager(void);

	void deviceInsertedOrRemoved(int vid, int pid, const std::string &serial, bool inserted);

	Controller *getBestControllerById(int id);

protected:
	void loadControllers();

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLERMANAGER_H
