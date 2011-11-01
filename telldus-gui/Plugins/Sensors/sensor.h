#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QMetaType>
#include <QVariantMap>
#include <QDateTime>

class SensorValue;

class Sensor : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool hasHumidity READ hasHumidity NOTIFY hasHumidityChanged)
	Q_PROPERTY(bool hasTemperature READ hasTemperature NOTIFY hasTemperatureChanged)
	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
	Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString protocol READ protocol WRITE setProtocol NOTIFY protocolChanged)
	Q_PROPERTY(bool showInList READ showInList NOTIFY showInListChanged)

public:
	explicit Sensor(QObject *parent = 0);
	~Sensor();

	bool hasHumidity() const;

	int id() const;
	void setId(int id);

	QString model() const;
	void setModel(const QString &model);

	QString name() const;
	//void setName(const QString &name);

	QString protocol() const;
	void setProtocol(const QString &protocol);

	bool hasTemperature() const;
	bool showInList() const;

	Q_INVOKABLE SensorValue *sensorValue(int type);
	Q_INVOKABLE void setValue(int type, const QString &value, const QDateTime &timestamp);
	Q_INVOKABLE void setName(const QString &name);
	Q_INVOKABLE void setShowInList(bool show);

signals:
	void idChanged();
	void hasHumidityChanged();
	void hasTemperatureChanged();
	void modelChanged();
	void nameChanged();
	void protocolChanged();
	void showInListChanged();

private:
	class PrivateData;
	PrivateData *d;
};

Q_DECLARE_METATYPE(Sensor*)

#endif // SENSOR_H
