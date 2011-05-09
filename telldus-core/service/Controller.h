#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Event.h"
#include <string>

class ControllerEventData : public EventDataBase {
public:
	std::string msg;
	int controllerId;
};

class Controller {
public: 
	virtual ~Controller();

	virtual int firmwareVersion() = 0;
	virtual int send( const std::string &message ) = 0;

protected:
	Controller(int id, Event *event);
	void publishData(const std::string &data) const;
	void decodePublishData(const std::string &data) const;

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLER_H
