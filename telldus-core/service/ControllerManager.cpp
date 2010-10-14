#include "ControllerManager.h"
#include "Controller.h"
#include "TellStick.h"

#include <map>
#include <stdio.h>

typedef std::map<int, Controller *> ControllerMap;

class ControllerManager::PrivateData {
public:
	int lastControllerId;
	ControllerMap controllers;	//TODO: Lock for this AND lock in controls, for send etc
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
	if (!d->controllers.size()) {
		return 0;
	}
	ControllerMap::iterator it = d->controllers.find(id);
	if (it != d->controllers.end()) {
		return it->second;
	}
	return d->controllers.begin()->second;

}

void ControllerManager::loadControllers() {
	std::list<TellStickDescriptor> list = TellStick::findAll();
	std::list<TellStickDescriptor>::iterator it = list.begin();
	for(; it != list.end(); ++it) {
		printf("Found (%i/%i): %s\n", (*it).vid, (*it).pid, (*it).serial.c_str());
		TellStick *controller = new TellStick(*it);
		if (!controller->isOpen()) {
			delete controller;
			continue;
		}
		d->lastControllerId--;
		d->controllers[d->lastControllerId] = controller;
	}
	printf("List containing %i controllers\n", (int)d->controllers.size());
}
