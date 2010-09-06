#ifndef LIVEMESSAGE_H
#define LIVEMESSAGE_H

#include <QObject>
#include <QMetaType>

class LiveMessage : public QObject
{
	Q_OBJECT
public:
	LiveMessage(const QString &name);
	virtual ~LiveMessage();

	QByteArray toByteArray() const;

	static LiveMessage fromByteArray(const QByteArray &ba);

public slots:
	void append(const QString &argument);
	QByteArray argument(int index) const;
	QString name() const;

private:
	LiveMessage(const QList<QVariant> &args);
	class PrivateData;
	PrivateData *d;
};

Q_DECLARE_METATYPE(LiveMessage*)

#endif // LIVEMESSAGE_H
