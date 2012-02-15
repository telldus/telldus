#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

class Controller;
namespace TelldusCore {
	class Event;
}

#include <string>

class ControllerManager {
public:
	ControllerManager(TelldusCore::Event *event);
	~ControllerManager(void);

	void deviceInsertedOrRemoved(int vid, int pid, const std::string &serial, bool inserted);

	Controller *getBestControllerById(int id);
	void loadControllers();
	void queryControllerStatus();
	int resetController(Controller *controller);

	std::wstring getControllers() const;
	int setControllerValue(int id, const std::wstring &name, const std::wstring &value);

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLERMANAGER_H
