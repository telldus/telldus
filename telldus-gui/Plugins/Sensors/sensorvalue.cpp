#include "sensorvalue.h"

class SensorValue::PrivateData {
public:
	QString value;
	QDateTime lastUpdated;
};

SensorValue::SensorValue(QObject *parent) :
	QObject(parent)
{
	d = new PrivateData;
}

SensorValue::~SensorValue() {
	delete d;
}

QDateTime SensorValue::lastUpdated() const {
	return d->lastUpdated;
}

void SensorValue::setLastUpdated(const QDateTime &lastUpdated) {
	d->lastUpdated = lastUpdated;
	emit lastUpdatedChanged();
}

QString SensorValue::value() const {
	return d->value;
}

void SensorValue::setValue(const QString &value) {
	d->value = value;
	emit valueChanged();
}
