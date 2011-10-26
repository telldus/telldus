#include "sensor.h"

class Sensor::PrivateData {
public:
	bool hasTemperature, hasHumidity;
	int id;
	QString model, name, protocol, temperature, humidity;
	QDateTime lastUpdated;
};

Sensor::Sensor(QObject *parent) :
	QObject(parent)
{
	d = new PrivateData;
	d->hasTemperature = false;
	d->hasHumidity = false;
	d->id = 0;
}

Sensor::~Sensor() {
	delete d;
}

QString Sensor::humidity() const {
	return d->humidity;
}

void Sensor::setHumidity(const QString &humidity) {
	d->humidity = humidity;
	d->hasHumidity = true;
	emit humidityChanged();
	emit hasHumidityChanged();
}

bool Sensor::hasHumidity() const {
	return d->hasHumidity;
}

int Sensor::id() const {
	return d->id;
}

void Sensor::setId(int id) {
	d->id = id;
	emit idChanged();
}

QDateTime Sensor::lastUpdated() const {
	return d->lastUpdated;
}

void Sensor::setLastUpdated(const QDateTime &lastUpdated) {
	d->lastUpdated = lastUpdated;
	emit lastUpdatedChanged();
}

QString Sensor::model() const {
	return d->model;
}

void Sensor::setModel(const QString &model) {
	d->model = model;
	emit modelChanged();
}

QString Sensor::name() const {
	return d->name;
}

void Sensor::setName(const QString &name) {
	d->name = name;
	emit nameChanged();
}

QString Sensor::protocol() const {
	return d->protocol;
}

void Sensor::setProtocol(const QString &protocol) {
	d->protocol = protocol;
	emit protocolChanged();
}

QString Sensor::temperature() const {
	return d->temperature;
}

void Sensor::setTemperature(const QString &temperature) {
	d->temperature = temperature;
	d->hasTemperature = true;
	emit temperatureChanged();
	emit hasTemperatureChanged();
}

bool Sensor::hasTemperature() const {
	return d->hasTemperature;
}
