#include "Controller.h"

class Controller::PrivateData {
public:
	Event *event;
};

Controller::Controller(Event *event){
	d = new PrivateData;
	d->event = event;
}

Controller::~Controller(){
	delete d;
}

void Controller::publishData(const std::string &msg) const {
	ControllerEventData *data = new ControllerEventData;
	data->msg = msg;
	d->event->signal(data);
}
