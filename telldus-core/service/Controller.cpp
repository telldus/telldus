#include "Controller.h"
#include "Protocol.h"
#include "EventUpdateManager.h"
#include "Strings.h"

class Controller::PrivateData {
public:
	TelldusCore::EventRef event, updateEvent;
	int id, firmwareVersion;
};

Controller::Controller(int id, TelldusCore::EventRef event, TelldusCore::EventRef updateEvent){
	d = new PrivateData;
	d->event = event;
	d->updateEvent = updateEvent;
	d->id = id;
	d->firmwareVersion = 0;
}

Controller::~Controller(){
	delete d;
}

void Controller::publishData(const std::string &msg) const {
	ControllerEventData *data = new ControllerEventData;
	data->msg = msg;
	data->controllerId = d->id;
	d->event->signal(data);
}

void Controller::decodePublishData(const std::string &data) const {

	std::list<std::string> msgList = Protocol::decodeData(data);

	for (std::list<std::string>::iterator msgIt = msgList.begin(); msgIt != msgList.end(); ++msgIt){
		this->publishData(*msgIt);
	}
}

int Controller::firmwareVersion() const {
	return d->firmwareVersion;
}

void Controller::setFirmwareVersion(int version) {
	d->firmwareVersion = version;
	EventUpdateData *eventData = new EventUpdateData();
	eventData->messageType = L"TDControllerEvent";
	eventData->controllerId = d->id;
	eventData->eventState = TELLSTICK_DEVICE_CHANGED;
	eventData->eventChangeType = TELLSTICK_CHANGE_FIRMWARE;
	eventData->eventValue = TelldusCore::intToWstring(version);
	d->updateEvent->signal(eventData);
}
