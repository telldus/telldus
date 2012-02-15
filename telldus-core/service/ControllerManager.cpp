#include "ControllerManager.h"
#include "Controller.h"
#include "Mutex.h"
#include "TellStick.h"
#include "Log.h"
#include "Message.h"
#include "Strings.h"
#include "Settings.h"
#include "../client/telldus-core.h"

#include <map>
#include <stdio.h>

class ControllerDescriptor {
public:
	std::wstring name;
	int type;
	Controller *controller;
};

typedef std::map<int, ControllerDescriptor> ControllerMap;

class ControllerManager::PrivateData {
public:
	int lastControllerId;
	Settings settings;
	ControllerMap controllers;
	TelldusCore::Event *event;
	TelldusCore::Mutex mutex;
};

ControllerManager::ControllerManager(TelldusCore::Event *event){
	d = new PrivateData;
	d->lastControllerId = 0;
	d->event = event;
	this->loadStoredControllers();
	this->loadControllers();
}

ControllerManager::~ControllerManager() {
	for (ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
		if (it->second.controller) {
			delete( it->second.controller );
		}
	}
	delete d;
}

void ControllerManager::deviceInsertedOrRemoved(int vid, int pid, const std::string &serial, bool inserted) {
	if (vid == 0x0 && pid == 0x0) { //All
		if (inserted) {
			loadControllers();
		} else {
			//Disconnect all
			TelldusCore::MutexLocker locker(&d->mutex);
			while(d->controllers.size()) {
				ControllerMap::iterator it = d->controllers.begin();
				delete it->second.controller;
				d->controllers.erase(it);
			}
		}
		return;
	}
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
				if (!it->second.controller) {
					continue;
				}
				TellStick *tellstick = reinterpret_cast<TellStick*>(it->second.controller);
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
		return it->second.controller;
	}
	return d->controllers.begin()->second.controller;

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
			if (!cit->second.controller) {
				continue;
			}
			TellStick *tellstick = reinterpret_cast<TellStick*>(cit->second.controller);
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

		int controllerId = d->lastControllerId+1;
		TellStick *controller = new TellStick(controllerId, d->event, *it);
		if (!controller->isOpen()) {
			delete controller;
			continue;
		}
		d->lastControllerId = controllerId;
		d->controllers[d->lastControllerId].controller = controller;
		if (controller->pid() == 0x0c30) {
			d->controllers[d->lastControllerId].type = TELLSTICK_CONTROLLER_TELLSTICK;
		} else {
			d->controllers[d->lastControllerId].type = TELLSTICK_CONTROLLER_TELLSTICK_DUO;
		}
	}
}

void ControllerManager::loadStoredControllers() {
	int numberOfControllers = d->settings.getNumberOfNodes(Settings::Controller);
	TelldusCore::MutexLocker locker(&d->mutex);

	for (int i = 0; i < numberOfControllers; ++i) {
		int id = d->settings.getNodeId(Settings::Controller, i);
		d->controllers[id].controller = NULL;
		d->controllers[id].type = TELLSTICK_CONTROLLER_TELLSTICK;
		d->controllers[id].name = d->settings.getName(Settings::Controller, id);
	}
}

void ControllerManager::queryControllerStatus(){

	std::list<TellStick *> tellStickControllers;

	{
		TelldusCore::MutexLocker locker(&d->mutex);
		for(ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
			if (!it->second.controller) {
				continue;
			}
			TellStick *tellstick = reinterpret_cast<TellStick*>(it->second.controller);
			if (tellstick) {
				tellStickControllers.push_back(tellstick);
			}
		}
	}

	bool reloadControllers = false;
	std::string noop = "N+";
	for(std::list<TellStick *>::iterator it = tellStickControllers.begin(); it != tellStickControllers.end(); ++it) {
		int success = (*it)->send(noop);
		if(success == TELLSTICK_ERROR_BROKEN_PIPE){
			Log::warning("TellStick query: Error in communication with TellStick, resetting USB");
			resetController(*it);
		}
		if(success == TELLSTICK_ERROR_BROKEN_PIPE || success == TELLSTICK_ERROR_NOT_FOUND){
			reloadControllers = true;
		}
	}

	if(!tellStickControllers.size() || reloadControllers){
		//no tellstick at all found, or controller was reset
		Log::debug("TellStick query: Rescanning USB ports");  //only log as debug, since this will happen all the time if no TellStick is connected
		loadControllers();
	}
}

int ControllerManager::resetController(Controller *controller) {
	TellStick *tellstick = reinterpret_cast<TellStick*>(controller);
	if (!tellstick) {
		return true; //not tellstick, nothing to reset at the moment, just return true
	}
	int success = tellstick->reset();
	deviceInsertedOrRemoved(tellstick->vid(), tellstick->pid(), tellstick->serial(), false); //remove from list and delete
	return success;
}

std::wstring ControllerManager::getControllers() const {
	TelldusCore::MutexLocker locker(&d->mutex);

	TelldusCore::Message msg;

	msg.addArgument((int)d->controllers.size());

	for(ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
		msg.addArgument(it->first);
		msg.addArgument(it->second.type);
		msg.addArgument(it->second.name.c_str());
		msg.addArgument(it->second.controller ? 1 : 0);
	}
	return msg;
}

int ControllerManager::setControllerValue(int id, const std::wstring &name, const std::wstring &value) {
	TelldusCore::MutexLocker locker(&d->mutex);

	ControllerMap::iterator it = d->controllers.find(id);
	if (it == d->controllers.end()) {
		return TELLSTICK_ERROR_NOT_FOUND;
	}
	if (name == L"name") {
		it->second.name = value;
	} else {
		return TELLSTICK_ERROR_SYNTAX; //TODO: Is this the best error?
	}
	return TELLSTICK_SUCCESS;
}
