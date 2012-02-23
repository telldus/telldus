#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QMetaType>

class Controller : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool available READ available NOTIFY availableChanged)
	Q_PROPERTY(QString firmware READ firmware NOTIFY firmwareChanged)
	Q_PROPERTY(int id READ id NOTIFY idChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString serial READ serial NOTIFY serialChanged)
	Q_PROPERTY(int type READ type NOTIFY typeChanged())

public:
	explicit Controller(int id = 0, int type = 1, const QString &name = "", QObject *parent = 0);
	~Controller();

	bool available() const;
	void setAvailable(bool available);

	QString firmware() const;

	int id() const;

	QString name() const;
	void setName(const QString &name);

	void save();

	QString serial() const;

	Q_INVOKABLE void tryRemove();

	int type() const;

signals:
	void availableChanged();
	void firmwareChanged();
	void idChanged();
	void nameChanged();
	void serialChanged();
	void typeChanged();

private:
	class PrivateData;
	PrivateData *d;
};

Q_DECLARE_METATYPE(Controller*)

#endif // CONTROLLER_H
