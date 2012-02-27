#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Event.h"
#include <string>

class ControllerEventData : public TelldusCore::EventDataBase {
public:
	std::string msg;
	int controllerId;
};

class Controller {
public:
	virtual ~Controller();

	virtual int firmwareVersion() const;
	virtual int send( const std::string &message ) = 0;
	virtual int reset() = 0;

protected:
	Controller(int id, TelldusCore::EventRef event, TelldusCore::EventRef updateEvent);
	void publishData(const std::string &data) const;
	void decodePublishData(const std::string &data) const;
	void setFirmwareVersion(int version);

private:
	class PrivateData;
	PrivateData *d;
};

#endif //CONTROLLER_H
