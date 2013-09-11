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
	Q_PROPERTY(bool hasRainRate READ hasRainRate NOTIFY hasRainRateChanged)
	Q_PROPERTY(bool hasRainTotal READ hasRainTotal NOTIFY hasRainTotalChanged)
	Q_PROPERTY(bool hasWindDirection READ hasWindDirection NOTIFY hasWindDirectionChanged)
	Q_PROPERTY(bool hasWindAverage READ hasWindAverage NOTIFY hasWindAverageChanged)
	Q_PROPERTY(bool hasWindGust READ hasWindGust NOTIFY hasWindGustChanged)
	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
	Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString protocol READ protocol WRITE setProtocol NOTIFY protocolChanged)
	Q_PROPERTY(bool showInList READ showInList WRITE setShowInList NOTIFY showInListChanged)
	Q_PROPERTY(bool sendToLive READ sendToLive WRITE setSendToLive NOTIFY sendToLiveChanged)

public:
	explicit Sensor(QObject *parent = 0);
	~Sensor();

	bool hasHumidity() const;

	int id() const;
	void setId(int id);

	QString model() const;
	void setModel(const QString &model);

	QString name() const;

	QString protocol() const;
	void setProtocol(const QString &protocol);

	bool hasRainRate() const;
	bool hasRainTotal() const;
	bool hasTemperature() const;
	bool hasWindDirection() const;
	bool hasWindAverage() const;
	bool hasWindGust() const;
	bool showInList() const;
	bool sendToLive() const;

	Q_INVOKABLE QObject *sensorValue(int type);
	Q_INVOKABLE void setValue(int type, const QString &value, const QDateTime &timestamp);
	Q_INVOKABLE void setName(const QString &name);
	Q_INVOKABLE void setShowInList(bool show);
	Q_INVOKABLE void setSendToLive(bool send);

signals:
	void idChanged();
	void hasHumidityChanged();
	void hasRainRateChanged();
	void hasRainTotalChanged();
	void hasTemperatureChanged();
	void hasWindDirectionChanged();
	void hasWindAverageChanged();
	void hasWindGustChanged();
	void modelChanged();
	void nameChanged();
	void protocolChanged();
	void showInListChanged();
	void sendToLiveChanged();

private:
	class PrivateData;
	PrivateData *d;
};

Q_DECLARE_METATYPE(Sensor*)

#endif // SENSOR_H
