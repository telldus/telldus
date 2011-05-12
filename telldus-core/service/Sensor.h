#ifndef SENSOR_H
#define SENSOR_H

#include "Mutex.h"
#include <string>

class Sensor : public TelldusCore::Mutex
{
public:
	Sensor(const std::wstring &protocol, const std::wstring &model, int id);
	~Sensor();

	std::wstring protocol() const;
	std::wstring model() const;
	int id() const;

	void setValue(const std::string &name, const std::string &value, time_t timestamp);

private:
	class PrivateData;
	PrivateData *d;
};

#endif // SENSOR_H
