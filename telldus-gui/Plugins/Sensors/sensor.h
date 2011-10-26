#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QMetaType>
#include <QVariantMap>
#include <QDateTime>

class Sensor : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool hasHumidity READ hasHumidity NOTIFY hasHumidityChanged)
	Q_PROPERTY(bool hasTemperature READ hasTemperature NOTIFY hasTemperatureChanged)
	Q_PROPERTY(QString humidity READ humidity WRITE setHumidity NOTIFY humidityChanged)
	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
	Q_PROPERTY(QDateTime lastUpdated READ lastUpdated WRITE setLastUpdated NOTIFY lastUpdatedChanged)
	Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString protocol READ protocol WRITE setProtocol NOTIFY protocolChanged)
	Q_PROPERTY(QString temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
public:
	explicit Sensor(QObject *parent = 0);
	~Sensor();

	QString humidity() const;
	void setHumidity(const QString &humidity);
	bool hasHumidity() const;

	int id() const;
	void setId(int id);

	QDateTime lastUpdated() const;
	void setLastUpdated(const QDateTime &lastUpdated);

	QString model() const;
	void setModel(const QString &model);

	QString name() const;
	void setName(const QString &name);

	QString protocol() const;
	void setProtocol(const QString &protocol);

	QString temperature() const;
	void setTemperature(const QString &temperature);
	bool hasTemperature() const;

signals:
	void idChanged();
	void hasHumidityChanged();
	void hasTemperatureChanged();
	void humidityChanged();
	void lastUpdatedChanged();
	void modelChanged();
	void nameChanged();
	void protocolChanged();
	void temperatureChanged();

private:
	class PrivateData;
	PrivateData *d;
};

Q_DECLARE_METATYPE(Sensor*)

#endif // SENSOR_H
