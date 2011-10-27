#ifndef SENSORVALUE_H
#define SENSORVALUE_H

#include <QObject>
#include <QMetaType>
#include <QVariantMap>
#include <QDateTime>

class SensorValue : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QDateTime lastUpdated READ lastUpdated WRITE setLastUpdated NOTIFY lastUpdatedChanged)
	Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)

public:
	explicit SensorValue(QObject *parent = 0);
	~SensorValue();

	QDateTime lastUpdated() const;
	void setLastUpdated(const QDateTime &lastUpdated);

	QString value() const;
	void setValue(const QString &model);

signals:
	void lastUpdatedChanged();
	void valueChanged();

private:
	class PrivateData;
	PrivateData *d;
};

Q_DECLARE_METATYPE(SensorValue*)

#endif // SENSORVALUE_H
