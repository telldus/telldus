#include "Controller.h"

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
