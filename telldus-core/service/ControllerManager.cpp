#include "ControllerManager.h"
#include "Controller.h"
#include "Mutex.h"
#include "TellStick.h"
#include "Log.h"
#include "../client/telldus-core.h"

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
	Log::notice("Before for-loop");
	for(; it != list.end(); ++it) {
		Log::notice("Something in the loop");
		//Most backend only report non-opened devices.
		//If they don't make sure we don't open them twice
		bool found = false;
		ControllerMap::const_iterator cit = d->controllers.begin();
		for(; cit != d->controllers.end(); ++cit) {
			Log::notice("Something in the second loop");
			TellStick *tellstick = reinterpret_cast<TellStick*>(cit->second);
			if (!tellstick) {
				Log::notice("No tellstick");
				continue;
			}
			if (tellstick->isSameAsDescriptor(*it)) {
				found = true;
				Log::notice("FOUND");
				break;
			}
		}
		if (found) {
			continue;
		}

		int controllerId = d->lastControllerId-1;
		TellStick *controller = new TellStick(controllerId, d->event, *it);
		Log::notice("Is it open?");
		if (!controller->isOpen()) {
			Log::notice("Yes it was");
			delete controller;
			continue;
		}
		d->lastControllerId = controllerId;
		d->controllers[d->lastControllerId] = controller;
	}
}

void ControllerManager::queryControllerStatus(){

	std::list<TellStick *> tellStickControllers;

	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
			Log::notice("found a controller");
			TellStick *tellstick = reinterpret_cast<TellStick*>(it->second);
			if (tellstick) {
				Log::notice("found a tellstick");
				tellStickControllers.push_back(tellstick);
			}
		}
	}

	bool reloadControllers = false;
	std::string noop = "noop";
	for(std::list<TellStick *>::iterator it = tellStickControllers.begin(); it != tellStickControllers.end(); ++it) {
		int success = (*it)->send(noop);
		if(success == TELLSTICK_ERROR_COMMUNICATION){
			Log::warning("TellStick query: Error in communication with TellStick, resetting USB");
			resetController(*it);
			Log::notice("has reset");
		}
		if(success == TELLSTICK_ERROR_COMMUNICATION || success == TELLSTICK_ERROR_NOT_FOUND){
			reloadControllers = true;
			Log::notice("Set reload");
		}
	}

	if(!tellStickControllers.size() || reloadControllers){
		//no tellstick at all found, or controller was reset
		Log::warning("TellStick query: Rescanning USB ports");
		loadControllers();
	}
}

int ControllerManager::resetController(Controller *controller) {
	TellStick *tellstick = reinterpret_cast<TellStick*>(controller);
	if (!tellstick) {
		return true; //not tellstick, nothing to reset at the moment, just return true
	}
	Log::notice("resettingController");
	int success = controller->reset();
	Log::notice("Remove device");
	deviceInsertedOrRemoved(tellstick->vid(), tellstick->pid(), tellstick->serial(), false); //remove from list and delete
	Log::notice("Device removed");
	return success;
}
