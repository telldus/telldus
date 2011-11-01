#include "sensor.h"
#include "sensorvalue.h"
#include <telldus-core.h>
#include <QDebug>

class Sensor::PrivateData {
public:
	bool hasTemperature, hasHumidity, showInList;
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
	//return QString("%1 %2").arg(this->protocol()).arg(this->id()); //TODO: Remove when name is fully implemented
	if(d->name == ""){
		return "<unnamed>";
	}
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

bool Sensor::hasTemperature() const {
	return d->values.contains(TELLSTICK_TEMPERATURE);
}

SensorValue * Sensor::sensorValue(int type) {
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
	}
}

bool Sensor::showInList() const{
	//TODO showInList and name must be persistent...
	return d->showInList;
}

void Sensor::setShowInList(bool show){
	d->showInList = show;
	emit showInListChanged();
}
