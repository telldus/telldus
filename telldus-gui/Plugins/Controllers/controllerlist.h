#ifndef CONTROLLERLIST_H
#define CONTROLLERLIST_H

#include <QAbstractListModel>
#include <QScriptValue>
#include <telldus-core.h>

class ControllerList : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(int length READ rowCount)
public:
	explicit ControllerList(QObject *parent = 0);
	~ControllerList();

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
	void changed();
	void controllerEventSignal(int controllerId, int changeEvent, int changeType, const QString &newValue);

public slots:
	void save();

private slots:
	void controllerEventSlot(int controllerId, int changeEvent, int changeType, const QString &newValue);

private:
	static void WINAPI controllerEvent( int controllerId, int changeEvent, int changeType, const char *newValue, int callbackId, void *context);
	class PrivateData;
	PrivateData *d;

};

#endif // CONTROLLERLIST_H
