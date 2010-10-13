#include "ControllerManager.h"
#include "Controller.h"

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
}

ControllerManager::~ControllerManager(void) {
	for (ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
		delete( it->second );
	}
	delete d;
}

void ControllerManager::loadControllers() {
	//TODO: Do something
}
