#include "ControllerManager.h"
#include "Controller.h"
#include "Mutex.h"
#include "TellStick.h"

#include <map>
#include <stdio.h>

typedef std::map<int, Controller *> ControllerMap;

class ControllerManager::PrivateData {
public:
	int lastControllerId;
	ControllerMap controllers;	//TODO: lock in controls, for send etc
	TelldusCore::Mutex mutex;
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

void ControllerManager::deviceInsertedOrRemoved(int vid, int pid, bool inserted) {
	TelldusCore::MutexLocker locker(&d->mutex);
	//TODO:
}

Controller *ControllerManager::getBestControllerById(int id) {
	TelldusCore::MutexLocker locker(&d->mutex);
	if (!d->controllers.size()) {
		return 0;
	}
	ControllerMap::const_iterator it = d->controllers.find(id);
	if (it != d->controllers.end()) {
		return it->second;
	}
	return d->controllers.begin()->second;

}

void ControllerManager::loadControllers() {
	TelldusCore::MutexLocker locker(&d->mutex);
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
