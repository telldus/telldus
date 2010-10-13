#include "ControllerManager.h"
#include "Controller.h"
#include "TellStick.h"

#include <map>

typedef std::map<int, Controller *> ControllerMap;

class ControllerManager::PrivateData {
public:
	int lastControllerId;
	ControllerMap controllers;
};

ControllerManager::ControllerManager(){
	d = new PrivateData;
	d->lastControllerId = 0;
	this->loadControllers();
}

ControllerManager::~ControllerManager(void) {
	for (ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
		delete( it->second );
	}
	delete d;
}

Controller *ControllerManager::getBestControllerById(int id) {
	return 0;
}

void ControllerManager::loadControllers() {
	while(1) {
		TellStick *controller = TellStick::findFirstDevice();
		if (!controller) {
			break; //All TellStick loaded
		}
		//Make sure this isn't already loaded
		bool found = false;
		for(ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
			TellStick *tellstick = reinterpret_cast<TellStick *>(it->second);
			if (!tellstick) { //No TellStick controller
				continue;
			}
			if (tellstick->serial().compare(controller->serial()) == 0) { //Found a duplicate
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
		d->lastControllerId--;
		d->controllers[d->lastControllerId] = controller;
	}
}
