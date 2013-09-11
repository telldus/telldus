#include "sensor.h"
#include "sensorvalue.h"
#include <telldus-core.h>
#include <QDebug>

class Sensor::PrivateData {
public:
	bool hasTemperature, hasHumidity, hasRainRate, hasRainTotal, hasWindDirection, hasWindAverage, hasWindGust, showInList;
	int id;
	QString model, name, protocol;
	QDateTime lastUpdated;
	QMap<int, SensorValue *> values;
};

Sensor::Sensor(QObject *parent) :
	QObject(parent)
{
	d = new PrivateData;
	d->id = 0;
	d->showInList = false;
}

Sensor::~Sensor() {
	delete d;
}

bool Sensor::hasHumidity() const {
	return d->values.contains(TELLSTICK_HUMIDITY);
}

int Sensor::id() const {
	return d->id;
}

void Sensor::setId(int id) {
	d->id = id;
	emit idChanged();
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
	if (name == d->name) {
		return;
	}
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

bool Sensor::hasRainRate() const {
	return d->values.contains(TELLSTICK_RAINRATE);
}

bool Sensor::hasRainTotal() const {
	return d->values.contains(TELLSTICK_RAINTOTAL);
}

bool Sensor::hasTemperature() const {
	return d->values.contains(TELLSTICK_TEMPERATURE);
}

bool Sensor::hasWindDirection() const {
	return d->values.contains(TELLSTICK_WINDDIRECTION);
}

bool Sensor::hasWindAverage() const {
	return d->values.contains(TELLSTICK_WINDAVERAGE);
}

bool Sensor::hasWindGust() const {
	return d->values.contains(TELLSTICK_WINDGUST);
}

QObject * Sensor::sensorValue(int type) {
	return (d->values.contains(type) ? d->values[type] : 0);
}

void Sensor::setValue(int type, const QString &value, const QDateTime &timestamp) {
	SensorValue *sensorValue;
	if (d->values.contains(type)) {
		sensorValue = d->values[type];
	} else {
		sensorValue = new SensorValue(this);
		d->values[type] = sensorValue;
	}
	sensorValue->setValue(value);
	sensorValue->setLastUpdated(timestamp);

	if (type == TELLSTICK_TEMPERATURE) {
		emit hasTemperatureChanged();
	} else if (type == TELLSTICK_HUMIDITY) {
		emit hasHumidityChanged();
	} else if (type == TELLSTICK_RAINRATE) {
		emit hasRainRateChanged();
	} else if (type == TELLSTICK_RAINTOTAL) {
		emit hasRainTotalChanged();
	} else if (type == TELLSTICK_WINDDIRECTION) {
		emit hasWindDirectionChanged();
	} else if (type == TELLSTICK_WINDAVERAGE) {
		emit hasWindAverageChanged();
	} else if (type == TELLSTICK_WINDGUST) {
		emit hasWindGustChanged();
	}
}

bool Sensor::showInList() const{
	return d->showInList;
}

void Sensor::setShowInList(bool show){
	d->showInList = show;
	emit showInListChanged();
}
