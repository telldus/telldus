#include "Sensor.h"
#include "common.h"

class Sensor::PrivateData {
public:
	std::wstring protocol, model;
	int id;
	time_t timestamp;
};

Sensor::Sensor(const std::wstring &protocol, const std::wstring &model, int id)
	:Mutex()
{
	d = new PrivateData;
	d->protocol = protocol;
	d->model = model;
	d->id = id;
}

Sensor::~Sensor() {
	delete d;
}

std::wstring Sensor::protocol() const {
	return d->protocol;
}

std::wstring Sensor::model() const {
	return d->model;
}

int Sensor::id() const {
	return d->id;
}

void Sensor::setValue(const std::string &name, const std::string &value, time_t timestamp) {
	//TODO: Do acctual storing of values
	d->timestamp = timestamp;
}
