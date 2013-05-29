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
	Q_PROPERTY(int type READ type NOTIFY typeChanged)
	Q_PROPERTY(bool upgradable READ upgradable NOTIFY upgradableChanged)
	Q_PROPERTY(qreal upgradeProgress READ upgradeProgress NOTIFY upgradeProgressChanged)
	Q_PROPERTY(int upgradeStep READ upgradeStep NOTIFY upgradeStepChanged)

public:
	explicit Controller(int id = 0, int type = 1, const QString &name = "", QObject *parent = 0);
	virtual ~Controller();

	bool available() const;
	virtual void setAvailable(bool available);

	QString firmware() const;
	void setFirmware(const QString &version);

	int id() const;

	QString name() const;
	void setName(const QString &name);

	void save();

	QString serial() const;

	Q_INVOKABLE void tryRemove();

	int type() const;

	bool upgradable() const;
	Q_INVOKABLE virtual void upgrade();
	qreal upgradeProgress();
	int upgradeStep() const;

signals:
	void availableChanged();
	void firmwareChanged();
	void idChanged();
	void nameChanged();
	void serialChanged();
	void typeChanged();
	void upgradableChanged();
	void upgradeProgressChanged();
	void upgradeStepChanged();
	void upgradeDone();

protected:
	virtual bool isUpgradable() const;
	void setUpgradeStep(int newStep);
	void setUpgradeProgress(qreal completed);

private:
	class PrivateData;
	PrivateData *d;
};

Q_DECLARE_METATYPE(Controller*)

#endif // CONTROLLER_H
