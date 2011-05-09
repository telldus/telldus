#include "Controller.h"
#include "Protocol.h"
#include "ControllerMessage.h"
#include <stdio.h> //TODO DEBUG

class Controller::PrivateData {
public:
	Event *event;
	int id;
};

Controller::Controller(int id, Event *event){
	d = new PrivateData;
	d->event = event;
	d->id = id;
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

	ControllerMessage dataMsg(data);
	std::wstring protocolName = dataMsg.protocol();
	std::string dataParam = dataMsg.getParameter("data");
	std::list<std::string> msgList = Protocol::decodeData(protocolName, dataParam);
	
	for (std::list<std::string>::iterator msgIt = msgList.begin(); msgIt != msgList.end(); ++msgIt){
		this->publishData(*msgIt);
	}
}