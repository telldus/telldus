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
	ControllerMap controllers;
	Event *event;
	TelldusCore::Mutex mutex;
};

ControllerManager::ControllerManager(Event *event){
	d = new PrivateData;
	d->lastControllerId = 0;
	d->event = event;
	this->loadControllers();
}

ControllerManager::~ControllerManager() {
	for (ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
		delete( it->second );
	}
	delete d;
}

void ControllerManager::deviceInsertedOrRemoved(int vid, int pid, const std::string &serial, bool inserted) {
	if (vid != 0x1781) {
		return;
	}
	if (pid != 0x0C30 && pid != 0x0C31) {
		return;
	}
	if (inserted) {
		loadControllers();
	} else {
		//Autodetect which has been disconnected
		TelldusCore::MutexLocker locker(&d->mutex);
		bool again = true;
		while(again) {
			again = false;
			for(ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
				TellStick *tellstick = reinterpret_cast<TellStick*>(it->second);
				if (!tellstick) {
					continue;
				}
				if (serial.compare("") != 0) {
					TellStickDescriptor tsd;
					tsd.vid = vid;
					tsd.pid = pid;
					tsd.serial = serial;
					if (!tellstick->isSameAsDescriptor(tsd)) {
						continue;
					}
				} else if (tellstick->stillConnected()) {
					continue;
				}

				d->controllers.erase(it);
				delete tellstick;
				again=true;
				break;
			}
		}
	}
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
		//Most backend only report non-opened devices.
		//If they don't make sure we don't open them twice
		bool found = false;
		ControllerMap::const_iterator cit = d->controllers.begin();
		for(; cit != d->controllers.end(); ++cit) {
			TellStick *tellstick = reinterpret_cast<TellStick*>(cit->second);
			if (!tellstick) {
				continue;
			}
			if (tellstick->isSameAsDescriptor(*it)) {
				found = true;
				break;
			}
		}
		if (found) {
			continue;
		}

		int controllerId = d->lastControllerId-1;
		TellStick *controller = new TellStick(controllerId, d->event, *it);
		if (!controller->isOpen()) {
			delete controller;
			continue;
		}
		d->lastControllerId = controllerId;
		d->controllers[d->lastControllerId] = controller;
	}
}
